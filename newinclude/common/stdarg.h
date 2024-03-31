#ifndef STDARG_H
#define STDARG_H

typedef struct {
    unsigned int gp_offset;
    void *reg_save_area;
} va_list;

void va_copy(va_list dest, va_list src){
    dest.gp_offset = src.gp_offset;
    dest.reg_save_area = src.reg_save_area;
}

typedef void* __gnuc_va_list;

#define __va_size(type) \
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(list, param){ list.gp_offset = sizeof(param); list.reg_save_area = &param; }
#define va_arg(list, type) (*(type*)(list.reg_save_area + (list.gp_offset += sizeof(type)) - sizeof(type)))
#define va_end(v) ((void)0)

#endif