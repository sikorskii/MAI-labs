package ru.aldes;

import lombok.experimental.UtilityClass;

@UtilityClass

public class ParserPatterns {
    String nameSurnamePattern = "^\\d\\sNAME\\s[а-яА-Я]+\\s/[а-яА-Я]+/$";

    String personIDPattern = "^\\d\\s@I\\d+@\\sINDI$";

    String familyPattern = "^\\d\\s@\\w+@\\sFAM$";

    String husbandPattern = "^\\d\\sHUSB\\s@\\w+@$";

    String wifePattern = "^\\d\\sWIFE\\s@\\w+@$";

    String childPattern = "^\\d\\sCHIL\\s@\\w+@$";

    String sexPattern = "^\\d\\sSEX\\s\\w$";
}
