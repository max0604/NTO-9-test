# NTO-9-test

На вход консольному приложению через параметры командной строки передаётся путь к
каталогу с файлами, имя файла с результатами и количество потоков для парсинга файлов (>=1 и
не включает в себя главный поток приложения). В каждом файле строка и набор разделителей.
Нужно разбить строку на подстроки по этим разделителям и вывести результат в выходной файл.
Парсить файлы надо в несколько потоков и результат должен быть помещен в один выходной
файл. Реализация должна быть полностью на STL/Boost и демонстрировать понимание ООП,
работу с потоками и объектами синхронизации (количество потоков для парсинга файлов не
должно сильно превышать количество логических ядер процессора).
Исключить использование голых указателей, заменяя их на стандартные контейнеры и Smart
Pointers. Важно чтобы код был читаем и в целом оформлен на коммерческом уровне.
Приветствуется использование стандартных алгоритмов (имеющихся в STL/Boost).
Входные данные. Первая строка входного файла – строка для разбора. Каждая последующая
строка содержит один разделитель.
Выходные данные. Информация по каждому файлу идет последовательно не пересекаясь с
информацией с других файлов. При этом не важно в каком порядке выводятся эти файлы с
информацией в выходном файле.
Пример
[Имя файла hd.txt]:
Подстока 1 файла 1
Подстрока 2 файла 1
Подстрока...
Подстрока n файла 1
[Имя файла ab.txt]:
Подстрока 1 файла 2
Подстрока 2 файла 2
Подстрока...
Подстрока m файла 2
