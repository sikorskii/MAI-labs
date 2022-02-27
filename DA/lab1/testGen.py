from random import randrange


# key - post index
# value - unsigned long long


OUTPUT_DATA_SIZE = 1_000_000
MAX_VALUE = 999999
MIN_VALUE = 0
MIN_V_VALUE = 0
MAX_V_VALUE = 2**64 - 1

result_data_file = open("cmake-build-debug/test.txt", "w")

for i in range(OUTPUT_DATA_SIZE):
    result_string =  str(f'{randrange(MIN_VALUE, MAX_VALUE):06}')
    result_string += '\t'
    result_string += str(randrange(MIN_V_VALUE, MAX_V_VALUE))
    result_string += '\n'
    result_data_file.write(result_string)


result_data_file.close()

