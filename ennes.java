import java.io.*;
import java.util.Scanner;

public class Main {
    private static Catalog catalog;
    private static Scanner inputScanner;
    private static boolean quit;

    public static void main(String[] args) {
        catalog = new Catalog();
        quit = false;
        inputScanner = new Scanner(System.in);
        while (!quit) {
            int action = IOHandler.getUserInt(inputScanner, "Vul een nummer in om aan te geven wat u wilt doen:\n" +
                    "1 - Laat alle maaltijden in het voedseldagboek zien.\n" +
                    "2 - Voeg een maaltijd toe aan deze dag.\n" +
                    "3 - Zet uw calorieën doel.\n" +
                    "4 - Zet de datum.\n" +
                    "5 - Sla deze dag op in een bestand.\n" +
                    "6 - Laad een dag uit een bestand.\n" +
                    "7 - Sluit de applicatie af.");
            switch (action) {
                case 1 -> System.out.println(catalog.toString());
                case 2 -> addFood();
                case 3 -> setCalorieenGoal();
                case 4 -> setDate();
                case 5 -> saveCatalog();
                case 6 -> loadCatalog();
                case 7 -> closeApplication();
            }
        }
    }



    private static void addFood() {
        String name = IOHandler.getUserString(inputScanner, "Geef de naam van de maaltijd:", false);
        int amount = IOHandler.getUserInt(inputScanner, "Geef het aantal dat je de maaltijd gegeten hebt:");
        double calorieen = IOHandler.getUserDouble(inputScanner, "Geef het aantal calorieën van 1 maaltijd:");
        catalog.addFood(name, calorieen, amount);
    }

    private static void setCalorieenGoal() {
        double goal = IOHandler.getUserDouble(inputScanner, "Geef het calorieën doel:");
        catalog.setCalorieenGoal(goal);
    }

    private static void setDate() {
        int day = IOHandler.getUserInt(inputScanner, "Geef de dag:");
        int month = IOHandler.getUserInt(inputScanner, "Geef de maand:");
        int year = IOHandler.getUserInt(inputScanner, "Geef het jaar:");
        catalog.setDate(day, month, year);
    }

    private static void saveCatalog() {
        if (catalog.getDate() == null) {
            System.out.println("U moet eerst een datum invullen.");
            setDate();
        }
        try {
            FileOutputStream fileOut = new FileOutputStream(catalog.getDate().toString() + ".ser");
            ObjectOutputStream objOut = new ObjectOutputStream(fileOut);
            objOut.writeObject(catalog);
            objOut.close();
            fileOut.close();
            System.out.println("Het voedseldagboek is opgeslagen in: " + catalog.getDate().toString() + ".ser");
        } catch (FileNotFoundException e) {
            System.out.println("Het voedseldagboek kon niet worden opgeslagen.");
        } catch (IOException e) {
            System.out.println("Het voedseldagboek kon niet worden opgeslagen.");
        }
    }

    private static void loadCatalog() {
        String name = IOHandler.getUserString(inputScanner, "Geef het pad naar het bestand op inclusief de extensie:", false);
        try {
            FileInputStream fileIn = new FileInputStream(name);
            ObjectInputStream objIn = new ObjectInputStream(fileIn);
            catalog = (Catalog) objIn.readObject();
            objIn.close();
            fileIn.close();
            System.out.println("Het voedseldagboek is geladen.");
        } catch (FileNotFoundException e) {
            System.out.println("Het voedseldagboek kon niet worden geladen.");
        } catch (IOException e) {
            System.out.println("Het voedseldagboek kon niet worden geladen.");
        } catch (ClassNotFoundException e) {
            System.out.println("Het voedseldagboek kon niet worden geladen.");
        }
    }

    private static void closeApplication() {
        boolean save = IOHandler.getUserBoolean(inputScanner, "Wilt u het voedseldagboek opslaan? ja/nee");
        if (save)
            saveCatalog();
        quit = true;
    }

}
