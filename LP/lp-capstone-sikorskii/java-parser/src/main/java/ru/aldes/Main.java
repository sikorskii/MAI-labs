package ru.aldes;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Main {
    public static void main(String[] args) throws IOException {

        var inputFile = new File("input.ged");
        var inputFileReader = new FileReader(inputFile);
        var bufferedReader = new BufferedReader(inputFileReader);


        Map<String, Person> persons = new HashMap<>();

        int personNumber = 0;

        String personID = "";

        String husbandID = "";

        String wifeID = "";

        String line = bufferedReader.readLine();

        while (!line.matches(ParserPatterns.familyPattern)) {

            if (line.matches(ParserPatterns.personIDPattern)) {
                personID = line.replace("0 @I", "").
                        replace("@ INDI", "");
                persons.put(personID, new Person());
                persons.get(personID).children = new ArrayList<>();
            }

            if (line.matches(ParserPatterns.nameSurnamePattern)) {
                persons.get(personID).name = line.replace("1 NAME ", "").
                        replace("/", "");
                personNumber++;
            }

            if (line.matches(ParserPatterns.sexPattern)) {
                String sexLine = line.replace("1 SEX ", "");
                persons.get(personID).sex = sexLine.equals("M") ?
                        Sex.MALE : Sex.FEMALE;
            }

            line = bufferedReader.readLine();
        }

        while (line != null) {

            if (line.matches(ParserPatterns.husbandPattern)) {
                husbandID = line.replace("1 HUSB @I", "").
                        replace("@", "");
                System.out.println(husbandID);
            }

            if (line.matches(ParserPatterns.wifePattern)) {
                wifeID = line.replace("1 WIFE @I", "").
                        replace("@", "");
                System.out.println(wifeID);
            }

            if (line.matches(ParserPatterns.childPattern)) {
                String childKey = line.replace("1 CHIL @I", "").
                        replace("@", "");
                persons.get(husbandID).children.add(persons.get(childKey).name);
                persons.get(wifeID).children.add(persons.get(childKey).name);
            }

            line = bufferedReader.readLine();
        }

        System.out.println(personNumber);

        var outputFile = new File("output.pl");

        if (!outputFile.exists()) {
            if (!outputFile.createNewFile()) {
                System.out.println("unable to create output file\n");
                System.exit(1);
            }
        }

        var outputFileWriter = new FileWriter(outputFile);

        var bufferedOutputFileWriter = new BufferedWriter(outputFileWriter);


        for (String x : persons.keySet()) {
            Person toWrite = persons.get(x);
            if (toWrite.sex == Sex.MALE) {
                bufferedOutputFileWriter.write("male('" + toWrite.name + "').\n");
            }
            else {
                bufferedOutputFileWriter.write("female('" + toWrite.name + "').\n");
            }
        }

        bufferedOutputFileWriter.newLine();
        bufferedOutputFileWriter.newLine();

        for (String x : persons.keySet()) {
            Person toWrite = persons.get(x);
            System.out.println("ID: " + x + " Name: " + toWrite.name +
                    " Number of children: " + toWrite.children.size() +
                    " Sex: " + toWrite.sex);
            for (var child : toWrite.children) {
                System.out.println(child);
                bufferedOutputFileWriter.write("child('" + child + "', '" +
                        toWrite.name + "').\n");
            }
        }

        bufferedOutputFileWriter.flush();

        bufferedOutputFileWriter.close();
        outputFileWriter.close();
        inputFileReader.close();
        bufferedReader.close();
    }
}
