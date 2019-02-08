try:
    a = int(input("Число 1: "))
    b = int(input("Число 2: "))
    c = a / b + 12
except ValueError:
    c = "Это не число! Выходим из программы"
except ZeroDivisionError:
    c = "На ноль делить нельзя! Выход"
except Exception as ex:
    c = "Ошибка: " + str(ex)
finally:
    print(c)
