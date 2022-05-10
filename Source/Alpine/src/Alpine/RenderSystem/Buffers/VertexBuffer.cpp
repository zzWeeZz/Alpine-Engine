#include "VertexBuffer.h"

template<class T>
Alpine::VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& aBuffer)
{
	m_Buffer = aBuffer.m_Buffer;
	m_BufferSize = aBuffer.m_BufferSize;
	m_Stride = aBuffer.m_Stride;

}



