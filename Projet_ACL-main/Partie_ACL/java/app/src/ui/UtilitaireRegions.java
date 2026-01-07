package src.ui;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Classe utilitaire pour la gestion des régions.
 * Elle permet de récupérer les informations des régions stockées dans les fichiers JSON.
 */
public class UtilitaireRegions {
    /**
     * Chemin vers le dossier Région contenant les fichiers JSON des données de régions.
     */
    private static final String REGION_DIR = "Partie_ACL/Assets/Region/";

    /**
     * Extension des fichiers.
     */
    private static final String EXT = ".json";

    /**
     * Retourne le chemin du fichier contenant les données de la région passée en paramètre.
     * @param region - Une région.
     * @return - Chemin du fichier contenant les données de la région passée en paramètre.
     */
    public static String getCheminFichierRegion(String region) {
        return REGION_DIR + region + EXT;
    }

    /**
     * Retourne la liste des noms de régions (sans extension).
     * @return - Liste des noms des régions sans extension.
     */
    public static List<String> getNomsRegions() {
        List<String> regions = new ArrayList<>(); // Liste des régions à retourner.
        File dir = new File(REGION_DIR); // Chargement du fichier.

        // Si le dossier existe et qu'il s'agit d'un dossier, on parcourt les fichiers.
        if (dir.exists() && dir.isDirectory()) {
            for (File file : Objects.requireNonNull(dir.listFiles())) {
                if (file.isFile() && file.getName().endsWith(EXT)) {
                    // Récupérer le nom du fichier (nom de la région sans extension).
                    String nom = file.getName().substring(0, file.getName().length() - EXT.length());
                    regions.add(nom);
                }
            }
        }
        return regions;
    }
}

