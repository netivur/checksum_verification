# checksum_verification
## Использование  
	user$ csvp.exe путь_к_файлу алгоритм_поиска_контрольной_суммы(Не обязательно. crc32 по умолчанию).  
	**Важно:** программа поддерживает входные файлы только с расширением ".json". Структуру входного файла(inputjsonexample) см. в папке filesfortests.
	Пути до исследуемых файлов, хранящиеся во входном файле, должны быть указаны соответственно расположению файлов в Вашей системе. 
	При изменении расположения входных файлов измените пути до них в файле CMakeLists.txt (метка "#Tests").
	
	Исходные файлы программы хранятся в папке soursescsvp.
	В папке tempscsvp хранятся временные файлы сборки проекта и debug версия программы csvp.exe (подпапка Debug).