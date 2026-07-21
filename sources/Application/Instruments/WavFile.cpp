#include "WavFile.h"
#include "System/Console/Trace.h"
#include "Foundation/Types/Types.h"
#include "Application/Model/Config.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int WavFile::bufferChunkSize_=-1 ;
bool WavFile::initChunkSize_=true ;

short Swap16(short from) {
#ifdef __ppc__
	short result;
	((char*)&result)[0] = ((char*)&from)[1];
	((char*)&result)[1] = ((char*)&from)[0];
	return result;
#else
	return from;
#endif
}

int Swap32(int from) {
#ifdef __ppc__
	int result;
	((char*)&result)[0] = ((char*)&from)[3];
	((char*)&result)[1] = ((char*)&from)[2];
	((char*)&result)[2] = ((char*)&from)[1];
	((char*)&result)[3] = ((char*)&from)[0];
	return result;
#else
	return from;
#endif
}

static unsigned short ReadLE16(const unsigned char *data) {
	return (unsigned short)(data[0] | (data[1] << 8)) ;
}

static unsigned int ReadLE32(const unsigned char *data) {
	return (unsigned int)data[0] |
		((unsigned int)data[1] << 8) |
		((unsigned int)data[2] << 16) |
		((unsigned int)data[3] << 24) ;
}

WavFile::WavFile(I_File *file) {
	if (initChunkSize_) {
		const char *size=Config::GetInstance()->GetValue("SAMPLELOADCHUNKSIZE") ;
		if (size) bufferChunkSize_=atoi(size) ;
		initChunkSize_=false ;
	}
	samples_=0 ;
	size_=0 ;
	sampleBufferSize_=0 ;
	file_=file ;
} ;

WavFile::~WavFile() {
	if (file_) {
		file_->Close() ;
		delete file_ ;
	}
	SAFE_FREE(samples_) ;
} ;

bool WavFile::readExact(void *destination,long count) {
	unsigned char *output=(unsigned char *)destination ;
	while (count>0) {
		int request=count>INT_MAX?INT_MAX:(int)count ;
		int bytesRead=file_->Read(output,1,request) ;
		if (bytesRead<=0 || bytesRead>request) return false ;
		output+=bytesRead ;
		count-=bytesRead ;
	}
	return true ;
} ;

bool WavFile::readAt(long position,void *destination,long count) {
	file_->Seek(position,SEEK_SET) ;
	return readExact(destination,count) ;
} ;

WavFile *WavFile::Open(const char *path) {
	FileSystem *fs=FileSystem::GetInstance() ;
	I_File *file=fs->Open(path,(char *)"r") ;
	if (!file) return 0 ;

	WavFile *wav=new WavFile(file) ;
	file->Seek(0,SEEK_END) ;
	long fileSize=file->Tell() ;
	if (fileSize<12) {
		Trace::Error("WAV file is truncated") ;
		delete wav ;
		return 0 ;
	}

	unsigned char header[12] ;
	if (!wav->readAt(0,header,sizeof(header)) ||
		memcmp(header,"RIFF",4)!=0 || memcmp(header+8,"WAVE",4)!=0) {
		Trace::Error("Bad RIFF/WAVE format") ;
		delete wav ;
		return 0 ;
	}

	bool gotFormat=false ;
	bool gotData=false ;
	unsigned int dataSize=0 ;
	long position=12 ;
	while (position<=fileSize-8) {
		unsigned char chunkHeader[8] ;
		if (!wav->readAt(position,chunkHeader,sizeof(chunkHeader))) break ;

		unsigned int chunkSize=ReadLE32(chunkHeader+4) ;
		long chunkData=position+8 ;
		unsigned long chunkEnd=(unsigned long)chunkData+chunkSize ;
		if (chunkEnd>(unsigned long)fileSize || chunkEnd<(unsigned long)chunkData) {
			Trace::Error("WAV chunk is truncated") ;
			delete wav ;
			return 0 ;
		}

		if (memcmp(chunkHeader,"fmt ",4)==0) {
			unsigned char format[16] ;
			if (chunkSize<sizeof(format) ||
				!wav->readAt(chunkData,format,sizeof(format))) {
				Trace::Error("Bad WAV fmt chunk") ;
				delete wav ;
				return 0 ;
			}

			unsigned short compression=ReadLE16(format) ;
			unsigned short channels=ReadLE16(format+2) ;
			unsigned int sampleRate=ReadLE32(format+4) ;
			unsigned short blockAlign=ReadLE16(format+12) ;
			unsigned short bitsPerSample=ReadLE16(format+14) ;
			if (compression!=1 || (channels!=1 && channels!=2) ||
				(bitsPerSample!=8 && bitsPerSample!=16) || sampleRate==0 ||
				blockAlign!=channels*(bitsPerSample/8)) {
				Trace::Error("Unsupported WAV format (PCM mono/stereo 8/16-bit required)") ;
				delete wav ;
				return 0 ;
			}

			wav->sampleRate_=sampleRate ;
			wav->channelCount_=channels ;
			wav->bytePerSample_=bitsPerSample/8 ;
			gotFormat=true ;
		} else if (memcmp(chunkHeader,"data",4)==0) {
			wav->dataPosition_=chunkData ;
			dataSize=chunkSize ;
			gotData=true ;
		}

		if (gotFormat && gotData) break ;
		unsigned long next=chunkEnd+(chunkSize&1) ;
		if (next>(unsigned long)LONG_MAX) break ;
		position=(long)next ;
	}

	if (!gotFormat || !gotData) {
		Trace::Error("WAV is missing fmt or data chunk") ;
		delete wav ;
		return 0 ;
	}

	unsigned int frameSize=wav->channelCount_*wav->bytePerSample_ ;
	if (dataSize%frameSize!=0 || dataSize/frameSize>(unsigned int)INT_MAX) {
		Trace::Error("WAV sample data is too large or misaligned") ;
		delete wav ;
		return 0 ;
	}
	wav->size_=(int)(dataSize/frameSize) ;
	return wav ;
} ;

void *WavFile::GetSampleBuffer(int note) {
	return samples_ ;
} ;

int WavFile::GetSize(int note) {
	return size_ ;
} ;

int WavFile::GetChannelCount(int note) {
	return channelCount_ ;
} ;

int WavFile::GetSampleRate(int note) {
	return sampleRate_ ;
} ;

bool WavFile::GetBuffer(long start,long size) {
	if (start<0 || size<0 || start>size_ || size>size_-start) {
		Trace::Error("Invalid WAV sample range") ;
		return false ;
	}

	unsigned long sampleValues=(unsigned long)channelCount_*(unsigned long)size ;
	unsigned long requestedSize=sampleValues*sizeof(short) ;
	if (requestedSize>UINT_MAX) {
		Trace::Error("WAV sample is too large") ;
		return false ;
	}
	unsigned int sampleBufferSize=(unsigned int)requestedSize ;
	if (sampleBufferSize>sampleBufferSize_) {
		short *newSamples=(short *)SYS_MALLOC(sampleBufferSize) ;
		if (!newSamples) {
			Trace::Error("Failed to allocate %u bytes for WAV sample",sampleBufferSize) ;
			return false ;
		}
		SAFE_FREE(samples_) ;
		samples_=newSamples ;
		sampleBufferSize_=sampleBufferSize ;
	}

	unsigned long inputSize=sampleValues*(unsigned long)bytePerSample_ ;
	long inputPosition=dataPosition_+start*channelCount_*bytePerSample_ ;
	file_->Seek(inputPosition,SEEK_SET) ;
	unsigned char *output=(unsigned char *)samples_ ;
	unsigned long remaining=inputSize ;
	unsigned int chunkSize=bufferChunkSize_>0 ?
		(unsigned int)bufferChunkSize_ : 128*1024 ;
	if (chunkSize>(unsigned int)INT_MAX) chunkSize=INT_MAX ;
	while (remaining>0) {
		unsigned int request=remaining>chunkSize?chunkSize:(unsigned int)remaining ;
		int bytesRead=file_->Read(output,1,request) ;
		if (bytesRead<=0 || bytesRead>(int)request) {
			Trace::Error("Unexpected end of WAV sample data") ;
			SAFE_FREE(samples_) ;
			sampleBufferSize_=0 ;
			return false ;
		}
		output+=bytesRead ;
		remaining-=bytesRead ;
	}

	// Expand unsigned 8-bit PCM in place. Working backwards prevents source
	// bytes from being overwritten before they have been converted.
	if (bytePerSample_==1) {
		unsigned char *src=(unsigned char *)samples_ ;
		short *dst=samples_ ;
		for (long i=(long)sampleValues-1;i>=0;i--) {
			dst[i]=(short)(((int)src[i]-128)*256) ;
		}
	}
#ifdef __ppc__
	else {
		for (unsigned long i=0;i<sampleValues;i++) samples_[i]=Swap16(samples_[i]) ;
	}
#endif
	return true ;
} ;

void WavFile::Close() {
	if (!file_) return ;
	file_->Close() ;
	SAFE_DELETE(file_) ;
} ;

int WavFile::GetRootNote(int note) {
	return 60 ;
}
