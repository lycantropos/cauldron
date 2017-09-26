#pragma once

#include <iostream>
#include "../cauldron/utils.h"


bool is_character_in_string(char character,
                            const std::string &string);

bool is_lower(char character);

bool is_upper(char character);

bool is_digit(char character);

bool is_alphabetic(char character);

bool is_alphanumeric(char character);
