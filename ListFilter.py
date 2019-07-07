MIN_WORD_LENGTH = 4
MAX_WORD_LENGTH = 9
profanity_list = list()

def check_word(word):
    # length = len(word)
    # return length > MIN_WORD_LENGTH and length < MAX_WORD_LENGTH and word not in profanity_list
    # words already been filtered for length
    return word not in profanity_list

def main():
    with open('full-list-of-bad-words_text-file_2018_07_30.txt', 'r') as profanity_file:
        for line in profanity_file:    
            profanity_list.append(line)
    with open('WordListFinal2', 'r') as word_list:
        with open('WordListFinal3', 'w+') as filtered_word_list:
            for word in word_list:
                if check_word(word):
                    filtered_word_list.write(word)
                    #filtered_word_list.write('\n')

main()
