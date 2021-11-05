
% Task 2: Relational Data

% The line below imports the data
%:- [one.pl].


%utility

remove_all(_,[],[]).
remove_all(X,[X|L],L1) :- remove_all(X, L, L1).
remove_all(X,[Y|L],[Y|L1]) :- X \= Y, remove_all(X, L, L1).

make_set([],[]).
make_set([H|T],[H|T1]) :- remove_all(H,T,T2), make_set(T2,T1).


%Напечатать средний балл для каждого предмета

sum([], 0) .
sum([H|Y], N) :- sum(Y, N1), N is N1 + H.

average_mark(Sb, Avg) :- findall(X, grade(_,Sb, X), L), length(L,A), sum(L, B), Avg is B/A.

pick_sb([H|T]) :- average_mark(H, AVG), subject(H, I), print(I), nl, print(AVG), nl, pick_sb(T).

avg() :- findall(X, grade(_,X,_), L), make_set(L, L1), pick_sb(L1).


%Для каждой группы, найти количество не сдавших студентов

getmarks(X, L) :- findall(Z,grade(X,_,Z),L).

is_pass([]).
is_pass([H|T]) :- H > 2, is_pass(T).


count_students([], 0).
count_students([H|T], N) :- getmarks(H, L), is_pass(L) -> count_students(T, N);  count_students(T, M), N is M + 1.

gr_notpass(X,N) :- findall(Z,student(X,Z),L1), count_students(L1, N).

pick_gr([]).
pick_gr([H|T]) :- print(H), nl, gr_notpass(H, N), print(N), nl, pick_gr(T).

groups_notpass() :- findall(X, student(X,_), L), make_set(L, L1), pick_gr(L1).


%Найти количество не сдавших студентов для каждого из предметов

pick_st(X,[], 0).
pick_st(X,[H|T], N) :- grade(H, X, Z), Z < 3 -> pick_st(X, T, M), N is M + 1; pick_st(X, T, N).

sb_notpass(X, N) :-  findall(Z, grade(Z,X,_), L), pick_st(X, L, N).

pick_c([H|T]) :- subject(H, Z), print(Z), nl, sb_notpass(H, N), print(N), nl, pick_c(T).

s_notpass() :- findall(X, grade(H,X,_), L), make_set(L, L1), pick_c(L1).

student(102,'Петров').
student(101,'Петровский').
student(104,'Иванов').
student(102,'Ивановский').
student(104,'Запорожцев').
student(101,'Сидоров').
student(103,'Сидоркин').
student(102,'Биткоинов').
student(103,'Эфиркина').
student(103,'Сиплюсплюсов').
student(103,'Программиро').
student(104,'Джаво').
student(103,'Клавиатурникова').
student(101,'Мышин').
student(104,'Фулл').
student(101,'Безумников').
student(102,'Шарпин').
student(104,'Круглосчиталкин').
student(103,'Решетников').
student(102,'Эксель').
student(102,'Текстописов').
student(103,'Текстописова').
student(101,'Густобуквенникова').
student(102,'Криптовалютников').
student(104,'Блокчейнис').
student(102,'Азурин').
student(103,'Вебсервисов').
student(102,'Круглотличников').
subject('LP','Логическое программирование').
subject('MTH','Математический анализ').
subject('FP','Функциональное программирование').
subject('INF','Информатика').
subject('ENG','Английский язык').
subject('PSY','Психология').
grade('Петров','LP',4).
grade('Петров','MTH',3).
grade('Петров','FP',5).
grade('Петров','INF',4).
grade('Петров','ENG',4).
grade('Петров','PSY',3).
grade('Петровский','LP',5).
grade('Петровский','MTH',5).
grade('Петровский','FP',5).
grade('Петровский','INF',4).
grade('Петровский','ENG',5).
grade('Петровский','PSY',3).
grade('Иванов','LP',5).
grade('Иванов','MTH',3).
grade('Иванов','FP',4).
grade('Иванов','INF',5).
grade('Иванов','ENG',5).
grade('Иванов','PSY',5).
grade('Ивановский','LP',3).
grade('Ивановский','MTH',5).
grade('Ивановский','FP',4).
grade('Ивановский','INF',3).
grade('Ивановский','ENG',5).
grade('Ивановский','PSY',5).
grade('Запорожцев','LP',2).
grade('Запорожцев','MTH',2).
grade('Запорожцев','FP',4).
grade('Запорожцев','INF',3).
grade('Запорожцев','ENG',4).
grade('Запорожцев','PSY',4).
grade('Сидоров','LP',4).
grade('Сидоров','MTH',4).
grade('Сидоров','FP',4).
grade('Сидоров','INF',4).
grade('Сидоров','ENG',3).
grade('Сидоров','PSY',4).
grade('Сидоркин','LP',4).
grade('Сидоркин','MTH',5).
grade('Сидоркин','FP',4).
grade('Сидоркин','INF',3).
grade('Сидоркин','ENG',4).
grade('Сидоркин','PSY',4).
grade('Биткоинов','LP',5).
grade('Биткоинов','MTH',4).
grade('Биткоинов','FP',3).
grade('Биткоинов','INF',4).
grade('Биткоинов','ENG',3).
grade('Биткоинов','PSY',2).
grade('Эфиркина','LP',2).
grade('Эфиркина','MTH',4).
grade('Эфиркина','FP',3).
grade('Эфиркина','INF',2).
grade('Эфиркина','ENG',2).
grade('Эфиркина','PSY',4).
grade('Сиплюсплюсов','LP',4).
grade('Сиплюсплюсов','MTH',4).
grade('Сиплюсплюсов','FP',4).
grade('Сиплюсплюсов','INF',5).
grade('Сиплюсплюсов','ENG',4).
grade('Сиплюсплюсов','PSY',4).
grade('Программиро','LP',5).
grade('Программиро','MTH',4).
grade('Программиро','FP',3).
grade('Программиро','INF',3).
grade('Программиро','ENG',5).
grade('Программиро','PSY',4).
grade('Джаво','LP',4).
grade('Джаво','MTH',4).
grade('Джаво','FP',3).
grade('Джаво','INF',2).
grade('Джаво','ENG',3).
grade('Джаво','PSY',5).
grade('Клавиатурникова','LP',4).
grade('Клавиатурникова','MTH',5).
grade('Клавиатурникова','FP',3).
grade('Клавиатурникова','INF',5).
grade('Клавиатурникова','ENG',4).
grade('Клавиатурникова','PSY',4).
grade('Мышин','LP',5).
grade('Мышин','MTH',4).
grade('Мышин','FP',5).
grade('Мышин','INF',4).
grade('Мышин','ENG',3).
grade('Мышин','PSY',3).
grade('Фулл','LP',4).
grade('Фулл','MTH',4).
grade('Фулл','FP',4).
grade('Фулл','INF',5).
grade('Фулл','ENG',5).
grade('Фулл','PSY',5).
grade('Безумников','LP',3).
grade('Безумников','MTH',5).
grade('Безумников','FP',3).
grade('Безумников','INF',2).
grade('Безумников','ENG',4).
grade('Безумников','PSY',3).
grade('Шарпин','LP',3).
grade('Шарпин','MTH',4).
grade('Шарпин','FP',4).
grade('Шарпин','INF',5).
grade('Шарпин','ENG',5).
grade('Шарпин','PSY',5).
grade('Круглосчиталкин','LP',5).
grade('Круглосчиталкин','MTH',2).
grade('Круглосчиталкин','FP',4).
grade('Круглосчиталкин','INF',3).
grade('Круглосчиталкин','ENG',4).
grade('Круглосчиталкин','PSY',3).
grade('Решетников','LP',4).
grade('Решетников','MTH',4).
grade('Решетников','FP',5).
grade('Решетников','INF',3).
grade('Решетников','ENG',5).
grade('Решетников','PSY',3).
grade('Эксель','LP',4).
grade('Эксель','MTH',4).
grade('Эксель','FP',4).
grade('Эксель','INF',3).
grade('Эксель','ENG',5).
grade('Эксель','PSY',4).
grade('Текстописов','LP',2).
grade('Текстописов','MTH',5).
grade('Текстописов','FP',4).
grade('Текстописов','INF',3).
grade('Текстописов','ENG',5).
grade('Текстописов','PSY',3).
grade('Текстописова','LP',4).
grade('Текстописова','MTH',4).
grade('Текстописова','FP',4).
grade('Текстописова','INF',3).
grade('Текстописова','ENG',5).
grade('Текстописова','PSY',2).
grade('Густобуквенникова','LP',5).
grade('Густобуквенникова','MTH',2).
grade('Густобуквенникова','FP',4).
grade('Густобуквенникова','INF',5).
grade('Густобуквенникова','ENG',4).
grade('Густобуквенникова','PSY',3).
grade('Криптовалютников','LP',5).
grade('Криптовалютников','MTH',2).
grade('Криптовалютников','FP',2).
grade('Криптовалютников','INF',3).
grade('Криптовалютников','ENG',4).
grade('Криптовалютников','PSY',2).
grade('Блокчейнис','LP',5).
grade('Блокчейнис','MTH',2).
grade('Блокчейнис','FP',5).
grade('Блокчейнис','INF',3).
grade('Блокчейнис','ENG',4).
grade('Блокчейнис','PSY',5).
grade('Азурин','LP',4).
grade('Азурин','MTH',2).
grade('Азурин','FP',4).
grade('Азурин','INF',3).
grade('Азурин','ENG',5).
grade('Азурин','PSY',2).
grade('Вебсервисов','LP',3).
grade('Вебсервисов','MTH',4).
grade('Вебсервисов','FP',3).
grade('Вебсервисов','INF',3).
grade('Вебсервисов','ENG',5).
grade('Вебсервисов','PSY',2).
grade('Круглотличников','LP',4).
grade('Круглотличников','MTH',3).
grade('Круглотличников','FP',5).
grade('Круглотличников','INF',5).
grade('Круглотличников','ENG',3).
grade('Круглотличников','PSY',5).


