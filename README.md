# HOTWHEELS SPEED TRACK
Дети регулярно устраивают соревнования между собой - чья машинка быстрее и дальше уедет с горки. Больше всего у них машинок Hot Wheels в масштабе 1:64, отсюда родилась идея сделать трассу и измерять скорость машинки, скатывая их с одинакового склона. Машинки все похожи, но разгоняются по разному в зависимости от веса, размера и испорченности. Итоговый вариант собран на Arduino NANO. В качестве основы для для дорожного полотна использован кабель канал шириной 40 мм. Сверху машинку удерживают от скатывания сервоприводы. По команде заслоны открываются и одновременно две машинки скатываются с наклонной поверхности. Внизу установлены инфракрасные датчики. Излучатель и приемник разнесены: излучатель находится над трассой, приемник внизу под трассой, а в трассе сделано небольшое отверстие. Результат измерения выводится на ЖК-дисплей и дублируется для простоты понимания на два RGB светодиоды.
Для сборки понадобится:
* 1 х Кабель-канал 40x16 мм цвет белый, 2 м - 53р.
https://leroymerlin.ru/product/kabel-kanal-40x16-mm-cvet-belyy-82411065/
* 2 х Соединитель плоских каналов 55х110 - 58р.
https://leroymerlin.ru/product/soedinitel-ploskih-kanalov-55h110-18156185/
* 1 х Труба канализационная c шумопоглощением EQUATION D 50 мм L 0.25м полипропилен - 48р.
https://leroymerlin.ru/product/truba-kanalizacionnaya-c-shumopogloshcheniem-equation-d-50-mm-l-0-25m-polipropilen-81953276/
* 1 х Петля мебельная карточная Левша универсальная, 24х22 мм - 27р.
https://leroymerlin.ru/product/petlya-mebelnaya-kartochnaya-levsha-universalnaya-13181912/
* 1 х Arduino NANO - 155р.
https://aliexpress.ru/item/32989224656.html
* 2 х ИК-сенсор - 26р.
https://aliexpress.ru/item/32465285698.html
* 2 х Серво SG90 - 64р.
https://aliexpress.ru/item/32895148020.html
* 1 х Бипер, пассивный звуковой модуль - 49р.(2шт.)
https://aliexpress.ru/item/1893742787.html
* 1 х Красная кнопка - 95р.(5шт.)
https://aliexpress.ru/item/32704947684.html
* 1 х ЖК-дисплей - 143р.
https://aliexpress.ru/item/32652600313.html
* 2 х RGB светодиод - 547р.(50шт.)
https://aliexpress.ru/item/32584200777.html
* 1 х Mini USB разъем - 46р.(5шт.)
https://aliexpress.ru/item/4000216515466.html

Схема подключения:
![Схема подключения](https://github.com/dbprof/hot-wheels-speed-track/blob/master/schema.png)

Видео сборки:
[![Видео](https://github.com/dbprof/hot-wheels-speed-track/blob/master/video.png)](https://youtu.be/mLS2u-RiAHc)

Видео показ работы:
[![Видео](https://github.com/dbprof/hot-wheels-speed-track/blob/master/video2.png)](https://youtu.be/L8g52qN8nvk)
