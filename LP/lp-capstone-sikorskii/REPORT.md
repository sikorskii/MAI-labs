# Отчет по курсовому проекту
## по курсу "Логическое программирование"

### студент: <Сикорский Александр Александрович>

## Результат проверки

Вариант задания:

- [ ] стандартный, без NLP (на 3)
- [x] стандартный, с NLP (на 3-4)
- [] продвинутый (на 3-5)

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |
| Левинская М.А.|              |               |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*

## Введение

В ходе выполнения проекта мне нужно будет научиться писать парсер на каком-нибудь языке программирования. Скорее всего, это будет Java. Кроме этого необходимо составить файл родословной в формате `gedcom`, который и нужно будет распарсить к варианту представления для Пролога. На самом Прологе для начала нужно написать предикат поиска родственника, соответствующего варианту.


## Задание

Стандартное задание

1. Создать родословное дерево своего рода на несколько поколений (3-4) назад в стандартном формате [GEDCOM](https://ru.wikipedia.org/wiki/GEDCOM). Для этого рекомендуется использовать сервис [MyHeritage.com](http://myheritage.com), зарегистрировавшись в нем.

2. Преобразовать файл в формате GEDCOM в набор утверждений на языке Prolog, используя одно из следующих представлений в зависимости от номера в группе в соответствии с формулой ((N-1)+3) mod 4 + 1:
   3. с использованием предиката `child(ребенок, родитель)`, `male(человек)`, `female(человек)`

3. Реализовать предикат проверки/поиска следующих родственников в соответствии с номером варианта ((N-1) mod 7)+1:
   6. Теща
4. Реализовать программу на языке Prolog, которая позволит определять степень родства двух произвольных индивидуумов в дереве, например:
```prolog
?- relative(brother, ‘Петя’, X). 
X = ‘Вася’ 
?- relative(W,’Петя’,’Вася’). 
X = brother 
X = child – father     
% т.е. Вася является ребенком отца Пети 
X = child – mother  
% т.е. Вася является ребенком матери Пети 
```


## Получение родословного дерева

В моем дереве получилось 17 индивидумов, а само дерево я выдумал, вдохновившись именами и фамилиями из файла с реляционным представлением для первой лабораторной работы. В дереве не так много людей, но это достаточно, чтобы на нём работали предикаты проверки различных людей на разные степени родства. Дерево я создал на сервисе [MyHeritage.com](http://myheritage.com) и экспортировал файл `gedcom`

## Конвертация родословного дерева

Для того, чтобы преобразовать файл  `gedcom` в соответствующее варианту представление, я написал парсер на языке Java. Парсер состоит из трех классов, но по-хорошему нужно добавить еще для открытия файла с данными, создания потоков ввода-вывода. А тот код, который написан в Main'e, тоже нужно вынести в отдельный класс. Так будет красиво.

Парсер создает хеш-карту, где ключом является ID индивидума в дереве. ID представляется строкой, что обусловлено особенностью представления строк в Джаве и работой метода `.hashCode()`. Парсер проходится по всему файлу и с помощью regexp'ов рассматривает строки, в которых содержится интересующая нас информация. Так последовательно заполняются все данные об индивидуме. У каждого элемента `Person` в карте есть имя `name`, пол `sex` и список детей `children`. Когда весь файл с данными обработан, собирается вся информация из карты и пишется в файл в представлении, определенном вариантом.



## Предикат поиска родственника

Опишите, как устроен предикат поиска родственника, приведите его исходный код и примеры запросов/ответов (протокол работы).

Код предиката, описание:

```prolog

mother_in_law(Man, A) :- findall(X, child(X, Man), [Child|_]), % находим детей у человека. В данном представлении невозможно определить тёщу,
                                                               % если у человека нет детей, т.е мы не можем установить связь муж-жена.
                                                               % нам нужен только один ребенок, чтобы предикат не выполнял бектрекинг, если детей несколько.
                                                               % Он всё равно будет находить одну и ту же тёщу, так что оставляем только одного ребенка - голову
                                                               % списка. 
                                                               
                         child(Child, Y), female(Y),           % Зная ребенка мужа, находим мать ребенка - жену мужа
                         
                         child(Y, A), female(A), print(A).     % Зная жену, находим её мать.

```

Примеры запросов-ответов:

```prolog
?- mother_in_law('Сокет Шарпов', X).
'Система Микромягкая'
X = 'Система Микромягкая' ;
false.

?- mother_in_law('Лог Шарпов', X).
'Строка Виртуалова'
X = 'Строка Виртуалова'.

?- mother_in_law('Сервис Шарпов', X).
false.

?- mother_in_law('Сервис Шарпов', 'Строка Виртуалова').
false.

?- mother_in_law('Лог Шарпов', 'Строка Виртуалова').
'Строка Виртуалова'
true.

```

## Определение степени родства

Определение степени родства практически полностью основано на принципах и на коде третьей лабораторной работы.
Имеем предикат `prolong`, который ищет следующий шаг в пространстве состояний, а правила перехода в этом
пространстве заданы отношениями родства. Правилами описаны некоторые отношения, но можно найти и более длинные
пути в дереве, тогда пути будут составными, состоящими из нескольких "дуг". Уточню, что дугой в этом случае может
быть как отношение мать-сын, так и бабушка-внучка, так как некоторые связи "прыгают" через инвидидов. 

В остальном в курсовом проекте работает уже известный поиск с итерационным заглублением. Не в моем случае, но дерево 
родственников может быть довольно развесистым, так что я решил использовать преимущество именно этого метода решения, а
именно выигрыш в памяти.

```prolog
prolong([X|T], [Y, X|T]) :- swap(_, X, Y), not(member(Y, [X|T])).

%---------в отличие от 3 лабораторной (там предикат for вызывался перед запуском поиска по данной глубине),
%---------теперь переход на следующую глубину работает внутри предиката поиска с заглублением

get_next_positive(1).
get_next_positive(N) :- get_next_positive(N1), N is N1 + 1.

%---------в остальном реализация поиска с заглублением взята из лабораторной 3---------------------------------------

iteration_depth([Target|T], Target, [Target|T], 0).
iteration_depth(CurrentPath, Target, Path, N) :- N > 0, prolong(CurrentPath, NewPath), N1 is N-1, iteration_depth(NewPath, Target, Path, N1).

ifs(Start, Target, L) :- get_next_positive(N), iteration_depth([Start], Target, L, N), !.
```

## Утилитарные предикаты, которые облегчают жизнь

Создание математического множества из списка. В поиске родственников бывают такие ситуации, когда
в список родственников один и тот же индивид попадает несколько раз. Предположим, что клонов или полных
тёзок в семье не припоминается и удалим повторяющихся индивидов из списка. Реализация взята из первой лабораторной.


```prolog
remove_all(_,[],[]).
remove_all(X,[X|L],L1) :- remove_all(X, L, L1).
remove_all(X,[Y|L],[Y|L1]) :- X \= Y, remove_all(X, L, L1).

make_set([],[]).
make_set([H|T],[H|T1]) :- remove_all(H,T,T2), make_set(T2,T1).
```

Предикат `my_concat` используется в естественно-языковом интерфейсе, чтобы соединить имя и фамилию в единый терм.
Именно в виде таких термов в базе фактов представляются индивиды, например `female('База Шарпова').`
Использую встроенный в Пролог предикат. Сначала добавляю к имени пробел, затем дописываю фамилию.

```prolog
my_concat(P1, P2, P) :- concat(P1, ' ', PTemp), concat(PTemp, P2, P).
```

## Естественно-языковый интерфейс


Разбор выражений строится на словарях. Их тут много разных. Есть предикаты, которые переводят термы из множественного 
числа в единичное. Ведь поиску нет разницы, что грамматически "братьев много" отличается от "он у меня такой один".
Далее есть словари с некоторыми словами английского язка. Английская грамматика более строгая и её проще обрабатывать, 
нежели русскую. Словарики можно расширять, таким образом количество разных словесных обвязок одного и того же 
шаблона запроса будет расти. 

Сам интерфейс реализован предикатом, который постоянно крутится во время работы программы и ждет запрос от пользователя.
Для того, чтобы иметь возможность обращаться к человеку, упомянутому при предыдущем запросе, нужно прихранивать его
имя-фамилию в глобальной переменной. В курсе, насколько я помню, такие вещи не рассматривались, но в Прологе есть предикаты
`nb_getval(Name, Value)` (получает значение, связанное с глобальной переменной `Name` и унифицирует его с `Value`) и
`nb_setval(Name, Value)`, который позволяет проставить значение переменной.

Так вот, если в запросе мы рассматриваем какого-то одного индивида, его имя-фамилия заносятся в переменную и потом мы можем 
посмотреть, про кого мы пытались получить сведения в последний раз.

Запросы устроены следующим образом: на вход мы получаем фразу-список термов, каждый или почти каждый из которых мы должны проверить
на корректность(наличие такого слова в соответствующем словаре), после этого с помощью правил перехода узнаем информацию о родстве.
Запросы на естественном языке не позволят узнать длинные составные цепочки родства, так как вид родства приходит в запрос
одним термом. Наверное, можно было бы собирать составные цепочки из родственников и собирать терм конкатенацией, аналогично с тем, что
я делаю с именем-фамилией индивида, но это потребовало бы пересмотреть способ представления цепочки родства или хранения еще
одной цепочки, но в виде строки и реализации какого-то преобразования строки к списку(пути в пространстве состояний) и наоборот.
Так что для выяснения сложных связей предлагается использовать предикат `relative`.

```prolog
query([How, Many, Rel1, Does, P1, P2, Have, '?']) :- few_persons(Rel1, Rel), my_concat(P1, P2, P), how_dict(How), many_dict(Many), do_dict(Do), have_dict(Have),
    swap(Rel, _, _), nb_setval(last, P), findall(Q, swap(Rel, Q, P), T), !, make_set(T, T1), length(T1, N), write(P), write(" has "), write(N), write(" "), write(Rel),
    N > 1 -> writeln("s"), !; writeln(''), !.
```

Пример запроса. В этом запросе мы сначала получаем терм отношения в единственном числе, с которым уже можно работать, 
затем нужно получить индивида, объединив его имя с фамилией. После этого проверяем, всё ли в запросе корректно и соответстует ли
грамматике. Затем ищем требуемых родственников, проставляем индивида, с которым работаем в переменную `last`, после этого нужно получить множество 
родственников, убрав из решения повторяющихся. После этого идут формальности с выводом ответа. Глупо писать, к примеру, brothers вместо bro, если брат
нашел всего один-одинешенек. Проверяем, сколько родственников нашлось и по необходимости приписываем к слову s.

```prolog
?- interface.
|: how many brothers does Плюсоед Шарпов have?
Плюсоед Шарпов has 1 bro
|: how many sisters does Трекер Шарпов have?
Трекер Шарпов has 1 sister


|: how many brothers does База Шарпова have?
База Шарпова has 2 bros
|: how many sons does Лог Шарпов have?
Лог Шарпов has 2 sons



|: who is bro for База Шарпова?
bro for База Шарпова is 
Плюсоед Шарпов
Трекер Шарпов


|: who is Память Шарпова's husband?
Память Шарпова's husband is Сокет Шарпов
|: who is she for Сокет Шарпов?
she is wife for Сокет Шарпов

```


## Выводы

В ходе курсового проекта мне нужно было сделать довольно продвинутую систему, в которой сплелись несколько языков, типов 
представлений данных, алгоритмов и парадигм программирования. Для написания парсера файла GEDCOM я выбрал Java. Понятно, что 
это далеко не самый эффективный инструмент для подобной задачи, но в тот момент мне нужно было написть что-нибудь на 
этом языке, а тут подвернулся случай сделать что-то более-менее прикладное, а не просто решить очередную синтетическую задачку.
Уже после того, как парсер был готов, я узнал о том, что вообще-то всё нужно было делать не так и существуют средства, способные
выполнить ту же задачу и обойтись десятком строчек кода. 


Хочу отметить специфику представления данных, в силу которой просто невозможно "по-честному" найти связь брака между людьми, у 
которых нет детей. Это происходит потому, что связать мужа с женой мы можем только проверив отношение `child` для мужчины
и для женщины. Для решения подобной проблемы можно было бы добавить индивидов-затычек на листовые узлы дерева и всё стало бы на свои 
места, но идея такой модификации пришла мне в голову поздно. 


Практически везде, где я читал информацию для реферата упоминалось, что Пролог подходит и используется для анализа
естественных языков. Получилось применить язык и для этой цели, написав довольно комплексную систему, где нашли свое место
код и идеи из лабораторных работ, которые были в течение семестра.



