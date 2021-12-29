:- ['output.pl'].

%----------------------Задание варианта найти тёщу-----------------------------------------------------------------
mother_in_law(Man, A) :- findall(X, child(X, Man), [Child|_]),
                         child(Child, Y), female(Y),
                         child(Y, A), female(A), print(A).



%---------------------Правила перехода в пространстве состояния для определения степени родства-----------------------

swap(mom, A, B) :- child(B, A), female(A).
swap(dad, A, B) :- child(B, A), male(A).
swap(daughter, A, B) :- child(A, B), female(A).
swap(son, A, B) :- child(A, B), male(A).
swap(wife, A, B) :- child(C, A), child(C, B), female(A), male(B).
swap(husband, A, B) :- child(C, A), child(C, B), male(A), female(B).
swap(sister, A, B) :- child(A, C), child(B, C), female(A).
swap(bro, A, B) :- child(A, C), child(B, C), male(A), A \= B.
swap(grandma, A, B) :- child(C, A), child(B, C), female(A).
swap(granddad, A, B) :- child(C, A), child(B, C), male(A).
swap(granddaughter, A, B) :- child(A, C), child(C, B), female(A).
swap(grandson, A, B) :- child(A, C), child(C, B), male(A).

%----------------------Поиск с итерационным заглублением из лабораторной 3--------------------------------------------


prolong([X|T], [Y, X|T]) :- swap(_, X, Y), not(member(Y, [X|T])).

%---------в отличие от 3 лабораторной (там предикат for вызывался перед запуском поиска по данной глубине),
%---------теперь переход на следующую глубину работает внутри предиката поиска с заглублением

get_next_positive(1).
get_next_positive(N) :- get_next_positive(N1), N is N1 + 1.

%---------в остальном реализация поиска с заглублением взята из лабораторной 3---------------------------------------

iteration_depth([Target|T], Target, [Target|T], 0).
iteration_depth(CurrentPath, Target, Path, N) :- N > 0, prolong(CurrentPath, NewPath), N1 is N-1, iteration_depth(NewPath, Target, Path, N1).

ifs(Start, Target, L) :- get_next_positive(N), iteration_depth([Start], Target, L, N), !.

print_relation([]).
print_relation([H|T]) :- write(H), nl, print_relation(T).

relation_check([H, R], [W]) :- swap(W, R, H), !.
relation_check([H, R|T], TQ) :- relation_check([R|T], TY), swap(W, R, H), append(TY, [W], TQ), !.

relative(W, X, Y) :- (findall(Q, swap(W, Q, X), Y), !; ifs(X, Y, L), relation_check(L, W), print_relation(W)).


%-------------------------Грамматика и запросы-----------------------------------------------------------------------


%----------------------Словари можно расширить, добавив новые слова в список, с которым работает member---------------

few_persons(brothers, bro).
few_persons(daughters, daughter).
few_persons(sons, son).
few_persons(sisters, sister).
few_persons(husbands, husband).
few_persons(wives, wife).


he_dict(X) :- member(X,
 ['he',
  'his',
  'him'
  ]).

she_dict(X) :- member(X,
 ['she',
  'her'
  ]).

do_dict(X) :- member(X,
 ['do',
  'does',
  'did'
  ]).

be_dict(X) :- member(X,
 ['be',
  'is',
  'are'
  ]).

who_dict(X) :- member(X,
 ['who'
  ]).

how_dict(X) :- member(X,
 ['how'
  ]).

many_dict(X) :- member(X,
 ['many',
  'much'
  ]).

have_dict(X) :- member(X,
 ['has',
  'have'
  ]).

for_dict(X) :- member(X,
 ['for',
  's'
  ]).



%----------------------------Реализация некоторых запросов к системе + запуск программы и обработка запросов------------

%---------------------------Делаем из списка множество а-ля математическое, без повторяющихся элементов, как в первой лабораторной

remove_all(_,[],[]).
remove_all(X,[X|L],L1) :- remove_all(X, L, L1).
remove_all(X,[Y|L],[Y|L1]) :- X \= Y, remove_all(X, L, L1).

make_set([],[]).
make_set([H|T],[H|T1]) :- remove_all(H,T,T2), make_set(T2,T1).

%----------------------------------------вынес в отдельный предикат конкатенацию имени и фамилии индивида---------------

my_concat(P1, P2, P) :- concat(P1, ' ', PTemp), concat(PTemp, P2, P).

query([Who, Is, Rel, For, P1, P2, '?']) :- my_concat(P1, P2, P), who_dict(Who), be_dict(Is), for_dict(For), swap(Rel, _, _),
nb_setval(last, P), findall(Q, swap(Rel, Q, P), T), !, write(Rel), write(" for "), write(P), writeln(" is "), make_set(T, T1), print_relation(T1), !.

query([Who, Is, It, For, P1, P2, '?']) :- my_concat(P1, P2, P), who_dict(Who), be_dict(Is), for_dict(For), (he_dict(It);
    she_dict(It)), nb_getval(last, X), swap(Rel, X, P), !, write(It), write(" is "), write(Rel), write(" for "), writeln(P), !.

query([Do, P1, P2, Have, Rel, '?']) :- my_concat(P1, P2, P), do_dict(Do), have_dict(Have),
        swap(Rel, _, _), nb_setval(last, P), (swap(Rel, _, P) -> writeln("Yes"); not(swap(Rel, _, P)) -> writeln("No")), !.

query([Who, Is, P10, P11, For, P20, P21, '?']) :- my_concat(P10, P11, P1), my_concat(P20, P21, P2),
    who_dict(Who), be_dict(Is), for_dict(For), swap(Rel, P1, P2), write(P1), write(" for "), write(P2), write(" is "), writeln(Rel), !.

query([Who, Is, P1, P2, _, S, R, '?']) :- my_concat(P1, P2, P), who_dict(Who), be_dict(Is), for_dict(S), swap(R, _, _),
    nb_setval(last, P), findall(Q, (swap(Rel, Q, P)), T), !, write(P), write("'s "), write(Rel), write(" is "), make_set(T, T1), print_relation(T1), !.

query([How, Many, Rel1, Does, P1, P2, Have, '?']) :- few_persons(Rel1, Rel), my_concat(P1, P2, P), how_dict(How), many_dict(Many), do_dict(Do), have_dict(Have),
    swap(Rel, _, _), nb_setval(last, P), findall(Q, swap(Rel, Q, P), T), !, make_set(T, T1), length(T1, N), write(P), write(" has "), write(N), write(" "), write(Rel),
    N > 1 -> writeln("s"), !; writeln(''), !.


interface :- nb_setval(last, ''), repeat, readln(Line), ((query(Line), fail);(Line = [exit], !)).


?- query(['do', 'Плюсоед', 'Шарпов', 'have', 'daughter', '?']).





