#ifndef _OPTION_
#define _OPTION_
#include "Recievable.h"
#include <cstring>
#include <stdio.h>

#define OPTION_BUFFER_SIZE 400

// While there is a template here only int, float and string are supported
template<class T>
class Option : public Recievable<T>
{
private:
    T value;
    T default_value;
    const T* options;
    const size_t option_count;
    size_t options_json_size;
    char* format_spec;
protected:
    char* buffer;
    char* allocate_buffer() override;
public:
    // Constructor
    /**
     * Constructor for the Option class.
     * @param options A pointer to possible values for the option. The first element is used as the default value.
     */
    Option(const char* key, const T* options, size_t option_count);
    
    // Constructor
    Option(const char* key, const T* options, size_t option_count, int default_idx);
    
    size_t get_json_size() override;

    void cat_json(char* buffer, size_t buffer_size) override;
};

#endif