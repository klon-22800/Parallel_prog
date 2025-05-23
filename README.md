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


# Отчет ЛАБ.3
В данной лабораторной работе была реализована параллельная версия алгоритма умножения квадратных матриц с использованием библиотеки MPI [клик](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/matrix.cc). 

## Общий алгоритм

- Матрица A разделяется по строкам между всеми доступными процессами.
- Матрица B целиком рассылается (broadcast) всем участникам.
- Каждый процесс умножает свою подматрицу A на B и возвращает часть результата.
- Итоговая матрица собирается на процессе 0.
Распределение строк происходит с учётом остатка от деления `rows % world_size` для равномерной загрузки. Коммуникация реализована с помощью `MPI_Send`, `MPI_Recv` и `MPI_Bcast`.

## Результаты

### Выполнение на собственном компьютере.
![График](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/MPI_test.png)
Алгоритм показывает ожидаемый прирост на 4 потоках, однако на 12 не происходит значительного прироста, что может быть связано со слишком большими расходами на перессылку сообщений между потоками (что в будущем исправится при работа на кластере "Сергей Королев").

### Работа на суперкомпьютере "Сергей Королев"
![График](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/korolev_comprasion.png)
На суперкомпьютере алогритм показывает более стабильную работу (происходит практически трехкратное улучшение при увеличении кол-ва потоков в три раза). [Скрипт](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/script.pbs)
[Вывод 4 потока](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/slurm-124112.out) [Вывод_12 потоков](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/slurm-124114.out)

### P.S 
Еще должен быть тест на 20 потоках на суперкомпьютере, однако кто-то другой занял очередь на продолжительное время с более высоким приоритетом, когда очередь дойдет до меня добавлю графики в отчет. 
![Очередь](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_3/korolev/priority_error.jpg)

### PP.S за доступ к суперкомпьютеру спасибо [общажному жителю](https://github.com/IluhaZaz) ♥♥♥


# Отчет ЛАБ.4

Работу делали: [Сайдашев Андрей](https://github.com/klon-22800), [Сергеев Денис](https://github.com/Drowchik), [Русяев Алексей](https://github.com/Amitroki), [Зазвонов Илья](https://github.com/iluhazaz), [Ноздрякова Марина](https://github.com/NozdryakovaMarina)

В данной лабораторной работе была реализована параллельная версия алгоритма умножения квадратных матриц с использованием технологии CUDA.

## Общий алгоритм

- Каждый поток отвечает за рассчет одного элемента финальной матрицы.
- Для покрытия всей матрицы используется CUDA-сетка из блоков потоков

`dim3 block(blockSizeX, blockSizeY);`

`dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);`

Пример расчета: 

![Рассчет](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_4/python_check/formula1.png)

- Выделение памяти и загрузка с CPU на GPU

` cudaMalloc((void**)&d_A, size);`

`cudaMalloc((void**)&d_B, size);`

`cudaMalloc((void**)&d_C, size);`

`cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);`

`cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);`

- Далее запускается функция вычисления матрицы `matrixMulCUDA(const float* A, const float* B, float* C, int N) `

- Вычисляются индексы строки и столбца для каждого потока

` int row = blockIdx.y * blockDim.y + threadIdx.y;`

`int col = blockIdx.x * blockDim.x + threadIdx.x;`

где 

`blockIdx.x/y` - Индекс текущего блока по X/Y

`blockDim.x/y` - Размер блока (кол-во потоков в одном блоке)

`threadIdx.x/y` - Индекс потока внутри блока


## Результаты
![Результаты](https://github.com/klon-22800/Parallel_prog/blob/main/Lab_4/python_check/result.png)

## Выводы:

Расчеты были проведены на видеокарте RTX 4060, CUDA оказалась настолько хороша, что по итогу процесс вычисления замедлялся возможностями процессора по чтению матриц из файлов. Из графиков можно заметить, что увеличение размеров блоков сказывается положительно, за счет того, что требуется меньше ресурсов для их контроля, больше размерность = меньше количество блоков, но для матриц размера N < 5000, разница между block_size 16 и 32 становится несущественной. Вероятно разница была бы заметна на более больших размерах матрицы. 


