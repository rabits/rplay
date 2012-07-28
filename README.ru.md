rPlay - Рекурсивный Плеер
=========================

_Все проблемы, связанные с rPlay можно обсудить на [Issues Page](https://github.com/rabits/rplay/issues)_

## Предпосылки

*rPlay* - простой аудиоплеер, спроектированный для MeeGo 1.2: Harmattan.

Во-первых - rPlay является открытым ПО. Вы можете загрузить исходники, скомпилировать, собрать и запустить их на своем устройстве совершенно бесплатно.
С другой стороны, вы можете поддержать разработку rPlay и других моих программ, купив уже собранный пакет в [Nokia Ovi Store](http://store.ovi.com/content/282268).

Я создал rPlay из-за того, что N9 до сих пор не мог мне предоставить удобный файловый проигрыватель. Как обычно - музыкальная коллекция, составленная по метаданным композиций, не в полной мере отражает потребности в проигрывании музыки. Размещение композиций по папкам - гораздо более простой путь организации музыкальной библиотеки, чем редактирование метаданных и поддержание базы данных по композициям. Также наметилась тенденция на перегрузку проигрывателей излишним функционалом, который мне не нужен и откровенно мешает прослушиванию музыки.

![Выбранный трек](https://raw.github.com/wiki/rabits/rplay/screen2.png)

## Поддержать разработку

Если вы захотели поддержать разработку - это очень просто сделать при помощи Яндекс.Деньги или PayPal!

[Yandex.Money: 410011493978798](https://money.yandex.ru/donate.xml?to=410011493978798)
[PayPal](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&encrypted=-----BEGIN PKCS7-----MIIHJwYJKoZIhvcNAQcEoIIHGDCCBxQCAQExggEwMIIBLAIBADCBlDCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwDQYJKoZIhvcNAQEBBQAEgYBTFGFeNAg9e%2B3PPyOeaRZkNtpbu67aERV%2FdhWf%2FuJfHmUq%2FvuFKnk%2BC7SMssRoBzplsP4ULWFjpRPbQsBE0DU77AliCVUq6P%2FXyvwE15Pf4ToMphiCnpWGSa72bTOef%2FGW0LnzECt2Wsc33E3l0uqXKHe1%2BQDRUcuoR8blmwFmWjELMAkGBSsOAwIaBQAwgaQGCSqGSIb3DQEHATAUBggqhkiG9w0DBwQIAala9eWcRviAgYAv%2BTerpsTwakmU%2BTbC%2FtBLDNhwZUVdhZb60Hbb%2BfNTxXZ02itsCJdbEJR7OLa5KnBBB7oWnVaHvF8JTFLIwneO%2F0mkr8yTS8OhE9tU55ksc7VLxEW4JHUMDQAp6prBz%2F4FnhFabapYfFCJhT5ptacRN40v4PLhy5E95Scv%2FFtVrqCCA4cwggODMIIC7KADAgECAgEAMA0GCSqGSIb3DQEBBQUAMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1BheVBhbCBJbmMuMRMwEQYDVQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbTAeFw0wNDAyMTMxMDEzMTVaFw0zNTAyMTMxMDEzMTVaMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1BheVBhbCBJbmMuMRMwEQYDVQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAwUdO3fxEzEtcnI7ZKZL412XvZPugoni7i7D7prCe0AtaHTc97CYgm7NsAtJyxNLixmhLV8pyIEaiHXWAh8fPKW%2BR017%2BEmXrr9EaquPmsVvTywAAE1PMNOKqo2kl4Gxiz9zZqIajOm1fZGWcGS0f5JQ2kBqNbvbg2%2FZa%2BGJ%2FqwUCAwEAAaOB7jCB6zAdBgNVHQ4EFgQUlp98u8ZvF71ZP1LXChvsENZklGswgbsGA1UdIwSBszCBsIAUlp98u8ZvF71ZP1LXChvsENZklGuhgZSkgZEwgY4xCzAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsUCmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQADgYEAgV86VpqAWuXvX6Oro4qJ1tYVIT5DgWpE692Ag422H7yRIr%2F9j%2FiKG4Thia%2FOflx4TdL%2BIFJBAyPK9v6zZNZtBgPBynXb048hsP16l2vi0k5Q2JKiPDsEfBhGI%2BHnxLXEaUWAcVfCsQFvd2A1sxRr67ip5y2wwBelUecP3AjJ%2BYcxggGaMIIBlgIBATCBlDCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwCQYFKw4DAhoFAKBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTEyMDcyNzEwMjA0NVowIwYJKoZIhvcNAQkEMRYEFJcWjjo%2FAWEMNTSr7bEacgIzotypMA0GCSqGSIb3DQEBAQUABIGAJciJcvQGE12cUmSY7SnE17e3yhtxx8hhsXC8qwFF5Wejy2scxUKT0pShK%2BZsHiZe26B6yVamBOX6wcWGhFYqw7tOjQd%2Fqp8aIIQaycVoja7sq99NkUUC5iZQWT3ztOcjGfW9Fk0RaCZfsJD8lrKoTGU7mQnvzYFwEfVxMdjT%2B4Q%3D-----END PKCS7-----%0A)

## Использование

1. Установка rPlay на устройство:
 * Простая установка из [Nokia OVI Store](http://store.ovi.com/content/282268),
 * Или вы можете скачать исходники, скомпилировать их и установить на устройство вручную;
2. Запуск rPlay из главного меню;
3. Внимательно читаем всплывшее окошко с подсказками;
4. Выберите трек на странице Tree нажатием - и он будет проигран, нажмите на него еще раз - он остановится;
5. Налаживаем настройки под свою неповторимость;
6. Наслаждаемся.

## Устройство файловой библиотеки

*/Стиль/Исполнитель/[Год] Альбом/Номер - Название.mp3*

 - **Стиль**            - "Melodic Death Metal", "Thrash", "Death"...
 - **Исполнитель**      - "Carcass", "Death", "Arch Enemy"...
 - **[Год] Альбом**     - "[2005] The Spirit of Ukko", "[1994] Heartwork"...
 - **Номер - Название** - "04 - Heartwork", "08 - Children Of Bodom"...

## v1.0: базовый функционал

 * Простой и минималистичный интерфейс
 * Использование файловой системы (папок) в качестве музыкальной библиотеки
 * Бесконечное последовательное проигрывание
 * Показ обложек альбомов (cover.jpg, cover.png, folder.jpg, folder.png)
 * Поддержка ID3 и других типов метаданных
 * Поддержка гарнитур (Play/Pause, Forward, Rewind)
 * Поддержка текстов песен (трек.txt)

## v1.1

 * Оповещение голосом (Исполнитель, Альбом, Название текущего трека)

## v1.2

 * Режим перемешивания и повтора в выбранной директории
 * Автоматический поиск обложек и текстов в интернете
 * Гибкие настройки соединения с интернетом
 * Простой ручной поиск текстов в гугле
 * Редактор текстов песен
 * Минимальная локализация на русский язык
 * Добавлена простая помощь при старте программы и доступная из About
 * Возможность приостановить проигрывание и перейти на необходимую позицию в песне на странице Song
 * Нотификации о текущих скрытых операциях (загрузка обложек, текстов песен, синтез голоса)
 * Несколько некритичных фиксов и оптимизаций

## Планируемые возможности: v1.X => v2.0 

 * Поддержка Dolby Headphone
 * Информация о исполнителе (альбомы, вики)
 * Возможность отправлять тексты песен в открытую БД LyrDB.com
 * Использование PulseAudio для передачи звука на другую систему
 * Простой редактор файловой системы и тегов
 * Музыкальный будильник (выберет несколько альбомов и запустит их по таймеру)

## Особые функции

### Голосовой движок

В качестве голосового движка я использую flite. Сейчас синтез используется при изменении метаданных (Исполнитель, Альбом, Название текущей песни).

Для того, чтобы использовать синтез:
1. скомпилируйте flite 1.4 в scratchbox
2. Положите его библиотеки в rPlay/lib/flite/lib
3. Положите заголовочные файлы в rPlay/lib/flite/include
4. Пересоберите rPlay

### Загрузка обложек

Для загрузки обложек используется lastfm.com с метаданными из композиции (исполнитель, название альбома)

### Загрузка текстов песен

Для загрузки текстов используется lyrdb.com с метаданными из композиции (исполнитель, название трека) 

## Благодарности

Спасибо вам, Carnegie Mellon University за замечательный Flite: минималистичный движок синтеза голоса

## Автор

 * Rabit < home.rabits@gmail.com >

## Сайты

 * [GitHub/rPlay](https://github.com/rabits/rplay)
 * [Rabit.sru](http://rabits.ru)

## Лицензия: GNU GPLv2

    This package is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this package; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
