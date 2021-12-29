package ru.aldes;

import java.util.ArrayList;

enum Sex {
    MALE,
    FEMALE
}

public class Person {
    String name = "";
    ArrayList<String> children;
    Sex sex;
}
