# Отчет ЛАБ.1

## Описание 
Алгоритм произведения матриц написан на c++ [клик](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_1/include/matrix/matrix.h). Реализован класс Matrix, поддерживающий считывание матрицы из файла, произведение матриц и сохранение в файл .txt.
Верификация и визуализация работы написана на Python. [Алгоритм](https://github.com/klon-22800/Parallel_prog/blob/main/python_check/check.py) генерирует матрицы размера n*n со случайными значениями, записывает в файл, а затем вызывает .exe, после чего проверяет результат .exe с результатом numpy-метода .dot() и [логирует](https://github.com/klon-22800/Parallel_prog/blob/main/python_check/log.txt) скорость выполнения. 
Визуалицая представляет собой график зависимости скорость выполнения(ns)/размерность матрицы(n)

## Результат
![График](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_1/python_check/graphic.jpeg)


# Отчет ЛАБ.2

## Описание 
В данной лабораторной была использована библиотека OpenMP для многопоточного вычисления произведения матриц [клик](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_2/include/matrix/matrix.h). Параллельная обработка реализована в методе `dot()`, где строки результирующей матрицы вычсиляются разными потоками `#pragma omp parallel`, а векторизация используется для ускорения суммирования `#pragma omp simd reduction(+:sum)`. Алгоритм показал значительное увеличение производительности при работе с матрицами размера n > 100, на 12 потоках - примерно в 8 раз, на 4 потоках - примерно в 5 раз. Однако на матрицах меньшего размера однопоточный алгоритм показывает более высокие результаты. 
## Update
Ниже представлены графики сравнения однопоточного алгоритма, с алгоритмами на 12(максимально доступно) потоках и 4, кол-во потоков задавалось `#pragma omp parallel num_threads(4)`. 
## Результат
![График](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_2/python_check_2/log_omp.png)
![Сравнение](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_2/python_check_2/delta_percentage.png)
![Сравнение 4 потока](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_2/python_check_2/delta_percentage_4.png)
