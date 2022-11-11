#include "circular_buffer.h"

#include <assert.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



// The hidden definition of our circular buffer structure
struct circular_buf_t
{
    uint8_t     *   buffer;
    size_t          head;
    size_t          tail;
    size_t          max; //of the buffer
    bool            full;
};


cbuf_handle_t circular_buf_init(uint8_t* buffer, size_t size)
{
    assert(buffer && size);
    cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
    assert(cbuf);
    cbuf->buffer = buffer;
    cbuf->max = size;
    circular_buf_reset(cbuf);
    assert(circular_buf_empty(cbuf));
    return cbuf;
}



void circular_buf_reset(cbuf_handle_t cbuf)
{
    assert(cbuf);
    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
}



void circular_buf_free(cbuf_handle_t cbuf)
{
    assert(cbuf);
    free(cbuf);
}



bool circular_buf_full(cbuf_handle_t cbuf)
{
    assert(cbuf);
    return cbuf->full;
}



bool circular_buf_empty(cbuf_handle_t cbuf)
{
    assert(cbuf);
    return (!cbuf->full && (cbuf->head == cbuf->tail));
}



size_t circular_buf_capacity(cbuf_handle_t cbuf)
{
    assert(cbuf);
    return cbuf->max;
}



size_t circular_buf_size(cbuf_handle_t cbuf)
{
    assert(cbuf);
    size_t size = cbuf->max;
    if(!cbuf->full)
    {
        if(cbuf->head >= cbuf->tail)
        {
            size = (cbuf->head - cbuf->tail);
        }
        else
        {
            size = (cbuf->max + cbuf->head - cbuf->tail);
        }
    }
    return size;
}


static void advance_pointer(cbuf_handle_t cbuf)
{
    assert(cbuf);
    if(cbuf->full)
    {
        if (++(cbuf->tail) == cbuf->max) 
        { 
            cbuf->tail = 0;
        }
    }
    if (++(cbuf->head) == cbuf->max) 
    { 
        cbuf->head = 0;
    }
    cbuf->full = (cbuf->head == cbuf->tail);
}



static void retreat_pointer(cbuf_handle_t cbuf)
{
    assert(cbuf);
    cbuf->full = false;
    if (++(cbuf->tail) == cbuf->max) 
    { 
        cbuf->tail = 0;
    }
}



void circular_buf_put(cbuf_handle_t cbuf, uint8_t data)
{
    assert(cbuf && cbuf->buffer);
    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
}



int circular_buf_put2(cbuf_handle_t cbuf, uint8_t data)
{
    int r = -1;
    assert(cbuf && cbuf->buffer);
    if(!circular_buf_full(cbuf))
    {
        cbuf->buffer[cbuf->head] = data;
        advance_pointer(cbuf);
        r = 0;
    }
    return r;
}



int circular_buf_get(cbuf_handle_t cbuf, uint8_t * data)
{
    assert(cbuf);
    assert(data);
    assert(cbuf->buffer);
    assert(cbuf && data && cbuf->buffer);
    int r = -1;
    if(!circular_buf_empty(cbuf))
    {
        *data = cbuf->buffer[cbuf->tail];
        retreat_pointer(cbuf);
        r = 0;
    }
    return r;
}

void circular_buf_print(cbuf_handle_t cbuf)
{
        printf("beg:  ");
        for (int i = 0; i < cbuf->max; i++)
        {
            if (i == cbuf->tail)
                printf("v");
            else
                printf(" ");
        }
        printf("\n");

        printf("buf: [");
        for (int i = 0; i < cbuf->max; i++)
        {
            if (cbuf->full)
            {
                printf("%c", cbuf->buffer[i]);
            }
            else if (cbuf->tail <= cbuf->head)
            {
                if (i >= cbuf->tail && i < cbuf->head)
                {
                    printf("%c", cbuf->buffer[i]);
                }
                else
                {
                    printf(" ");
                }
            }
            else
            {
                if (i < cbuf->head || i >= cbuf->tail)
                {
                    printf("%c", cbuf->buffer[i]);
                }
                else
                {
                    printf(" ");
                }
            }
        }
        printf("]\n");

        printf("end:  ");
        for (int i = 0; i < cbuf->max; i++)
        {
            if (i == cbuf->head)
                printf("^");
            else
                printf(" ");
        }
        printf("\n");

        fflush(stdout);
}


