#include "CircularBuffer.h"

CircularBuffer :: CircularBuffer(size_t capacity) :
    _head(0),
    _tail(0),
    _buffer(NULL)
{
    _capacity = capacity;
    _buffer = new uint8_t[capacity];
    if(!_buffer)
        PRINT_ERR("Allocating Circular Buffer failed");
}

CircularBuffer :: ~CircularBuffer()
{
    if(_buffer)
        delete [] _buffer;
}

int CircularBuffer :: push(uint8_t* data)
{
    //Buffer is not full
    size_t head = _head;
    if(!(_tail == (head + 1)))
    {
        PRINT_DBG(2, "[CIRCULAR BUFF] PUSHING %c", *((char*)data));
        _buffer[head] = *data;
        if(++head == _capacity)
            _head = 0;
        else
            _head++;
        return CB_SUCCESS;
    }
    return CB_BUFFER_FULL;
}

int CircularBuffer :: pop(uint8_t* data)
{
    //Buffer is not Empty
    size_t tail = _tail;
    if(!(_head == tail))
    {
        PRINT_DBG(2, "[CIRCULAR BUFF] POPING %c", ((char)_buffer[tail]));
        *data = _buffer[tail];
        if(++tail == (_capacity - 1))
            _tail = 0;
        else
            _tail++;
        return CB_SUCCESS;      
    }
    return CB_BUFFER_EMPTY;
}

int CircularBuffer :: write(uint8_t* buffer, size_t size)
{
    if(!_buffer || !buffer || !size)
        return CB_ERROR;
    for(size_t i = 0; i < size; i++)
    {
        _rd_wr.lock();
        push(buffer + i);
        _rd_wr.unlock();
    }
    return CB_SUCCESS;
}

int CircularBuffer :: read(uint8_t* buffer, size_t size)
{
    int ret;
    if(!_buffer || !buffer || !size)
        return CB_ERROR;
    for(size_t i = 0; i < size; i++)
    {
        _rd_wr.lock();
        if((ret = pop(buffer + i)) != 0)
        {
            _rd_wr.unlock();
            PRINT_ERR("[CIRCULAR BUFF] pop errorno=%d", ret);
            return ret;
        }
        _rd_wr.unlock();
    }
    return CB_SUCCESS;
}

