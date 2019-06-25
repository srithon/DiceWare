from collections import defaultdict

def remove_duplicates(word_list):
    dictionary = defaultdict(list)
    
    for index, word in enumerate(word_list):
        dictionary[word].append(index)

    dictionary = { indexes[0] : word for word, indexes in dictionary.items() }

    return dictionary

def write_dict_to_file(processed_dictionary):
    with open('WordListFinal', 'w+') as final_word_list:
        print(processed_dictionary)
        for index in sorted(processed_dictionary):
            final_word_list.write(processed_dictionary[index])

def read_list():
    word_list = list()
    with open('FilteredWordListFull', 'r') as filtered_word_list:
        for word in filtered_word_list:
            word_list.append(word)
    return word_list

def main():
    words = read_list()
    write_dict_to_file(remove_duplicates(words))

main()
