#include <common/stdbool.h>
#include <common/stdint.h>

int32_t litoa(int64_t num, char* str) {
    int i = 0;
    int isNegative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return 0;
    }

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0;
    int end = strlen(str) - 1;
    char temp;
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return 0;
}

// Base method for itoa
int32_t _itoa(int32_t integer, char* result) {
	int num_digits = 0;
	int val = 0;
    // Push each digit
	while (integer > 0) {
        // Mod 10 gets just the last digit
        // Adding '0' turns it into an ASCII character
		val = integer % 10;
		if (val < 10) {
			result[num_digits] = val + '0';
		} else {
			result[num_digits] = val + 'a';
		}
		num_digits++;
        // Divide by 10 to get the next character in line
		integer /= 10;
	}

    // Reverse it and return it
    char tmp = 0;
    for (int i = 0; i < num_digits/2; i++) {
        // Swap at each end of the array
        tmp = result[i];
        result[i] = result[num_digits-i-1];
        result[num_digits-i-1] = tmp;
    }
    // Null terminate the string
	if (num_digits == 0) {
		num_digits = 1;
		result[0] = '0';
	}
    result[num_digits] = '\0';
	return 0;
}

int32_t itoa(int32_t integer, char* buffer) {
	return _itoa(integer, buffer);
}

int32_t itoah(int32_t num, char* buffer) {
    if (num == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return 0;
    }
    int i = 0;
    while (num > 0) {
        int remainder = num % 16;
        if (remainder < 10) {
            buffer[i++] = remainder + '0';
        } else {
            buffer[i++] = remainder - 10 + 'A';
        }
        num /= 16;
    }
    buffer[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    return 0;
}

int32_t itoab(int32_t num, char* buffer) {
    if (num == 0) {
        buffer[0] = '0';
		buffer[1] = '\0';
		return 0;
    }
    int i = 0;
    while (num > 0) {
        buffer[i++] = (num % 2) + '0';
        num /= 2;
    }
    buffer[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
	return 0;
}

bool strcmp(char* string1, char* string2) {
	int i = 0;
	while (true) {
		// Char mismatch? false
		if (string1[i] != string2[i]) {
			return false;
		}
		// They're equal, AND it's null char? We're done
		if (string1[i] == '\0') {
			break;
		}
		i++;
	}
	return true;
}