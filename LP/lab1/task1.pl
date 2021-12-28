my_member(X, [X|Y]).
my_member(X, [Y|Z]) :-
	my_member(X, Z).

my_length([], 0).
my_length([X|Y], T) :- my_length(Y, Q), T is Q + 1.

my_append([], L, L).
my_append([X | R1], L2, [X | R3]) :- my_append(R1, L2, R3).

my_remove(X, [X|R], R).
my_remove(X, [Y|R], [Y|T]) :- my_remove(X, R, T).

my_sublist(S, L) :- my_append(L1, L2, L), my_append(S, L3, L2).

my_permute([], []).
my_permute(L1, [X | R2]) :- my_remove(X, L1, L2), my_permute(L2, R2).


% Задание по удалению последнего элемента списка с помощью стандартных предикатов.
del_last(L, X):- append(X, [_], L).


% Это же задание без использования стандартных предикатов.
my_del([_], []).
my_del([X|L], [X|T]) :-
    my_del(L, T).


%Задание по вычислению числа четных элементов.my_del

is_even(N) :- N mod 2=:=0.

cnter([],0).
cnter([H|T],X):- is_even(H),!,cnter(T,X1), X is X1+1.
cnter([_|T],X):- cnter(T,X).


solution([], []) :- !.

solution([X], [X]) :- !.
solution([F, S|T], [S, F|OT]) :- solution(T, OT).


solution2([A1,A3, B1, B2, C2, C3]) :- my_permute([4, 5, 6, 7, 8, 9], [A1, A3, B1, B2, C2, C3]),
A1 + 1 + A3 =:= 15, B1 + B2 + 3 =:= 15, 2 + C2 + C3 =:= 15,
A1 + B1 + 2 =:= 15, 1 + B2 + C2 =:= 15, A3 + 3 + C3 =:= 15.

