#include <cs50.h>
#include <stdio.h>

int product_digit_sum(int digits[]) {
    return 0;
}

int digit_length(long long_num) {
    int length = 0;
    while (long_num > 0) {
        long_num = long_num / 10;
        length++;
    }

    return length;
}

int * long_to_digits(long long_num) {
    static int digits[16];
    int count = 0;
    while (long_num > 1){
        int r = long_num % 10;
        digits[count] = r;
        long_num = long_num / 10;
        count++;
    }

    return digits;
}

int sum_of_digits(int * arr, int length) {
    int i;
    int sum = 0;
    for(i = 0; i < length; i++) {
        if(arr[i] > 9) {
            sum = sum + arr[i] % 10 + arr[i] / 10;
        } else {
            sum = sum + arr[i];
        }
    }

    return sum;
}

bool is_valid_card(int * arr, int length) {
    bool is_valid = false;
    
    int i, j, loop;
    int odd[length / 2]; 
    int even[length / 2 + 1];
    for (i = 0; i < length; i++) {
        if (i % 2 == 0) {
            even[i / 2] = arr[i];
        } else {
            odd[i / 2] = arr[i];
        }
    }

    if (length % 2 == 0) {
        even[length / 2] = 0;
    }

    for (j = 0; j < length / 2; j++) {
        odd[j] = odd[j] * 2;
    }

    int sum = sum_of_digits(even, length / 2 + 1) + sum_of_digits(odd, length /2);

    if(sum % 10 == 0) {
        is_valid = true;
    }

    return is_valid;
}

enum Type {Invalid = 0, Amex = 1, Master = 2, Visa = 3}; 

int main(void) {
    enum Type type = Invalid; 
    long input = get_long("Number: ");
    int length = digit_length(input);

    int *digits = long_to_digits(input);

    if (length == 15 && digits[14] == 3 && (digits[13] == 4 || digits[13] == 7)) {
        type = Amex;
    } else if (length == 16 && digits[15] == 5 && (digits[14] >= 1 && digits[14] <= 5)) {
        type = Master;
    } else if (length == 13 && digits[12] == 4) {
        type = Visa;
    } else if (length == 16 && digits[15] == 4) {
        type = Visa;
    }

    bool is_valid = is_valid_card(digits, length);
    if (!is_valid) {
        type = Invalid;
    }

    switch(type) {
        case (Amex):
            printf("AMEX\n");
            break;
        case (Master):
            printf("MASTERCARD\n");
            break;
        case (Visa):
            printf("VISA\n");
            break;
        default:
            printf("INVALID\n");
            break;
    }
}
