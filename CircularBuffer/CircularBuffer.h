#ifndef _CIRCULAR_BUFFER_H
#define _CIRCULAR_BUFFER_H

#include <iostream>
#include <mutex>
#include <stdint.h>
#include <assert.h>
#include <cstdlib> //for getting env value

static int print_count;

#define PRINT_ERR(X...)      do {printf(X); printf(" [%d]\n", print_count++);}while(0)
#define PRINT_WARN(X...)     do {printf(X); printf(" [%d]\n", print_count++);}while(0)
#define PRINT_DBG(lvl, X...) do{\
                                char* s_dbg_lvl = getenv("CBUFF_DBG_LVL");\
                                int dbg_lvl = (s_dbg_lvl != NULL) ? atoi(s_dbg_lvl) : 0;\
                                if(dbg_lvl == lvl)\
                                {printf(X); printf(" [%d]\n", print_count++);}\
                                }while(0)

enum
{
    CB_SUCCESS = 0,
    CB_ERROR,
    CB_BUFFER_FULL,
    CB_BUFFER_EMPTY
};


class CircularBuffer
{

    public: 
        explicit                                            CircularBuffer(size_t capacity);
                                                            ~CircularBuffer();
        int                                                 write(uint8_t* buffer, size_t size);
        int                                                 read(uint8_t* buffer, size_t size);
        int                                                 getSize(void);
        int                                                 getErrStr(void);
    private:
        int                                                 push(uint8_t* data);
        int                                                 pop(uint8_t* data);

        size_t                                              _head;
        size_t                                              _tail;
        size_t                                              _capacity;
        std::mutex                                          _rd_wr;
        uint8_t*                                            _buffer;
};

#endif //file gaurd