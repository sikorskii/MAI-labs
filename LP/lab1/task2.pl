
% Task 2: Relational Data

% The line below imports the data
:- [one.pl].


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

