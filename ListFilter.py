MIN_WORD_LENGTH = 4
MAX_WORD_LENGTH = 9

def check_word(word):
    length = len(word)
    return length > MIN_WORD_LENGTH and length < MAX_WORD_LENGTH

def main():
    with open('WordListMedium', 'r') as word_list:
        with open('FilteredWordListMedium', 'w+') as filtered_word_list:
            for word in word_list:
                if check_word(word):
                    filtered_word_list.write(word)
                    #filtered_word_list.write('\n')

main()
