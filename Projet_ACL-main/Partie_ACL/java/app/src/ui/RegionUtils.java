package src.ui;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class RegionUtils {
    private static final String REGION_DIR = "Partie_ACL/Assets/Region/";
    private static final String EXT = ".json";

    // Retourne la liste des noms de régions (sans extension)
    public static List<String> getRegionNames() {
        List<String> regions = new ArrayList<>();
        File dir = new File(REGION_DIR);
        if (dir.exists() && dir.isDirectory()) {
            for (File file : dir.listFiles()) {
                if (file.isFile() && file.getName().endsWith(EXT)) {
                    String name = file.getName().substring(0, file.getName().length() - EXT.length());
                    name = name.replace("-", " ").replace("_", " "); // Optionnel : rendre plus lisible
                    regions.add(name);
                }
            }
        }
        return regions;
    }

    // Retourne le chemin complet du fichier JSON pour une région donnée
    public static String getRegionFilePath(String regionName) {
        // On suppose que le nom affiché correspond au nom du fichier sans extension
        String fileName = regionName.replace(" ", "-") + EXT;
        return REGION_DIR + fileName;
    }
}

