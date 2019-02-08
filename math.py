def check(number):
    if number < 100 or number > 999:
        return ["Это не трёхзначное число"]
    elif number % 10 == number // 10 % 10 or number // 10 % 10 == number // 100 % 10 or number % 10 == number // 100 % 10:
        return ["В числе есть одинаковые цифры"]
    else:
        return get_str_num(calc(number))

def get_hundred(nm):
    dict = {1:"сто", 2:"двести", 3:"триста", 4:"четыреста", 5:"пятьсот", 6:"шестьсот", 7:"семьсот", 8:"восемьсот", 9:"девятьсот"}
    return dict[nm]

def get_str_num(list):
    hundreds = {1:"сто", 2:"двести", 3:"триста", 4:"четыреста", 5:"пятьсот", 6:"шестьсот", 7:"семьсот", 8:"восемьсот", 9:"девятьсот"}
    tens = {2:"двадцать", 3:"тридцать", 4:"сорок", 5:"пятьдесят", 6:"шестьдесят", 7:"семьдесят", 8:"восемьдесят", 9:"девяносто"}
    ten = {0:"десять", 1:"одиннадцать", 2:"двенадцать", 3:"тринадцать", 4:"четырнадцать", 5:"пятнадцать", 6:"шестнадцать",
           7:"семнадцать", 8:"восемнадцать", 9:"девятнадцать"}
    units = {0:"", 1:"один", 2:"два", 3:"три", 4:"четыре", 5:"пять", 6:"шесть", 7:"семь", 8:"восемь", 9:"девять"}

    if list[1] >= 2:
        str_list = [hundreds[list[0]], tens[list[1]], units[list[2]]]
    elif list[1] == 1:
        str_list = [hundreds[list[0]], "", ten[list[2]]]
    elif list[1] == 0:
        str_list = [hundreds[list[0]], "", units[list[2]]]
    return str_list

def calc(number):
    d3 = number % 10         # единицы
    d2 = number // 10 % 10   # десятки
    d1 = number // 100 % 10  # сотни
    result = [d1, d2, d3]
    return result
about = ("math_python", "1.0.0.1", "Chip")
print("Программа:", about[0], "релиз:", about[1], "автор:", about[2])
while True:
    result_calc = []
    try:
        number = int(input("Введите трёхзначное число: "))
        result_calc.extend(check(number))
        #print("Сотни прописью: ", get_hundred(result_calc[0]))
    except KeyboardInterrupt:
        break
    except ValueError:
        result_calc.append("Это не число")
    except Exception as exError:
        result_calc.append("Ошибка выполнения программы: " + str(exError))
    finally:
        print(result_calc)
