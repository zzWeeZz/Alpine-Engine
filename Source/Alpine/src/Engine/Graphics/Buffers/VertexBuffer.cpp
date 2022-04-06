#include "VertexBuffer.h"

template<class T>
Alpine::VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& aBuffer)
{
	myBuffer = aBuffer.myBuffer;
	myBufferSize = aBuffer.myBufferSize;
	myStride = aBuffer.myStride;

}



