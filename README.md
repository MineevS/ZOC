# ZOC:
# @autor Mineev S. A.
# @group KMBO-02-19
# @data 21.09.2021

# 1. LAB № 0:
# 1.1 Требования:
#               1) Реализовать системную утилиту ls:
#						    $ ./ls
#						    $ ./ls path, где path - имя(имена) директорий.
#						    $ ./ls -l
#						    $ ./ls path -l
#						    $ ./ls -l path
#						    $ ./ls path -l path -l
#
#               2) Проверить лабораторную работу на кафедральном сервере !!!
#               3) Срок сдачи: 23.09.2021; (Зачтена: 25.09.2021[Невовремя])
#
# 1.2 Статус: Зачтена;

# 2. LA № 0.7:
# 1.1 Требования:
#               1) Реализовать:
#                        - без сжатия;
#                        $ ./archiver arch_name –i(--input) file1      // Добавление файлов в архив.
#                        $ ./archiver arch_name –e(--extract) file1    // Извлечение файлов из архива.
#                        $ ./archiver arch_name –s(--stat)             // вывести текущее состояние архива 
#                        $ ./archiver arch_name –h(--help)             // вывести справку по работе с архиватором
#					     $ ./archiver arch_name -d(--delete) file1 ... // Удаление файлов из архива.
#
#		2) Проверить лабораторную работу на кафедральном сервере !!!
#		3) Срок сдачи: 14.10.2021; (Зачтена: 15.11.2021[Невовремя])
#
# 1.2 Статус: Зачтена;

# 3. LAB № 1 -  fork() + atexit():
# 1.1 Требования:
#		1) Из текущего процесса создать дочерний.
#		2) Каждый процесс должен вывести на экран признак родительский/дочерний, pid, ppid.
#		3) Родительский процесс должен дождаться завершения дочернего.
#		4) Установить обработчик atexit(), чтобы при завершении процесса 
#                  выводилось сообщение о том, что сработал обработчик atexit(). 
#		5) Проверить лабораторную работу на кафедральном сервере !!!
#		6) Срок сдачи: 07.10.2021; (Зачтена:05.10.2021[Вовремя])
#
# 1.2 Статус: Зачтена;
#

# 4. LAB № 1.1:LAB_CHMOD
# 1.1 Требования:				-u(user - текущий пользователь)
#		1) Реализовать:			-g(group - группа)
#				./chmod path 	-o(ones - остальные)  -p(plus) -rwx
#						        -a(all - все)         -m(minus)
#  		2) -
#		3) -
#		4) -
#		5) Проверить лабораторную работу на кафедральном сервере !!!
#		6) Срок сдачи: 21.10.2021; (Зачтена:13.10.2021[Вовремя])
#
# 1.2 Статус: Зачтена;
#

# 5. LAB № 2 - pipe() + fifo():  
# 1.1 Требования:
		2a):
#		1) Создать pipe.
#  		2) Создать дочерний процесс.
#		3) Передать строчку через pipe из родительского в дочерний процесс с текущим временем и pid передающего процесса.
#		4) Дочерний процесс должен вывести на экран его текущее время и полученную через pipe строчку. 
#		5) Время из родительского и дочернего должно различаться как минимум на 5 секунд.
#		5) Проверить лабораторную работу на кафедральном сервере !!!
#		2b):
#		-) Аналогично 2а, но использовать fifo.
#		6) Срок сдачи: 25.11.2021; (Зачтена:15.11.2021[Вовремя])
#
# 1.2 Статус: Зачтена;
#

# 6. LAB_3 - ftok + shmeg +.. + shmctl:  
# 1.1 Требования:
#		1) Создать две независимых программы. Обязательно: вторую программу можно запускать параллельно несколько раз, 
#  		2) В бесконечном цикле передавать из первого процесса (первая программа)
#			через разделяемую память текущее время и pid передающего процесса.
#		3) Принимающий процесс (вторая программа) должен выводить на экран свое текущее время и pid и принятую строчку.
#		4) первая программа (передающая) должна запускать только один раз. 
#		5) Повторный запуск первой программа, если уже есть процесс с такой программой, 
#			должен завершаться с соответствующим сообщением на экране.
#		5) Проверить лабораторную работу на кафедральном сервере !!!
#		6) Срок сдачи: 02.12.2021; (Зачтена: В ожидании - 15.11.2021[Вовремя]).
#
# 1.2 Статус: Зачтена;
#

# 7. LAB_4 - semaphor:  
# 1.1 Требования:
#		1) Аналогично лабораторной работе №3, но для синхронизации обмена данными использовать semaphore. 
#		6) Срок сдачи: 09.12.2021; (Зачтена: В ожидании - 15.11.2021[Вовремя]).
#
# 1.2 Статус: -Зачтена;
#
# 8. LAB_5 - mutex:
# 1.1 Требования:
#		1) Создать 10 читающих потоков и 1 пишущий.   
#  		2) Потокам должен быть доступен один и тот же массив символов.
#		3) Пишущий поток записывает в общий массив номер записи (монотонно возрастающий счетчик), а читающие потоки выводят на экран свой tid и текущее состояние общего массива.
#		4) Доступ должен быть синхронизирован с помощью mutex.
#		5) Проверить лабораторную работу на кафедральном сервере !!!
#		6) Срок сдачи: 09.12.2021; (Зачтена: В ожидании - 15.11.2021[Вовремя]).
#
# 1.2 Статус: Зачтена;
#