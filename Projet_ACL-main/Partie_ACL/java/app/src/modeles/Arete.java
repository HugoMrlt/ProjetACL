package src.modeles;

/**
 * Classe représentant une arête.
 * Dans ce projet une arête représente une route qui lie deux villes.
 */
public class Arete {
    /**
     * Première extrémité (sommet).
     */
    private final Sommet sommet1;

    /**
     * Seconde extrémité (sommet).
     */
    private final Sommet sommet2;

    /**
     * Poids de l'arête (distance en km).
     */
    private final int poids;

    /**
     * Type de la route.
     */
    private final String typeRoute;

    /**
     * Identifiant du camion qui emprunte cette route.
     */
    private final int camionId;

    /**
     * Constructeur.
     * @param sommet1 - Première extrémité (ville).
     * @param sommet2 - Seconde extrémité (ville).
     * @param poids - Poids de l'arête (distance en km).
     * @param typeRoute - Type de la route.
     * @param camionId - Identifiant du camion qui emprunte cette route.
     */
    public Arete(Sommet sommet1, Sommet sommet2, int poids, String typeRoute, int camionId) {
        this.sommet1 = sommet1;
        this.sommet2 = sommet2;
        this.poids = poids;
        this.typeRoute = typeRoute;
        this.camionId = camionId;
    }

    /**
     * Retourne le premier sommet.
     * @return - Premier sommet.
     */
    public Sommet getSommet1() { return sommet1; }

    /**
     * Retourne le second sommet.
     * @return - Second sommet.
     */
    public Sommet getSommet2() { return sommet2; }

    /**
     *  Retourne le poids de l'arête.
     * @return - Poids de l'arête.
     */
    public int getPoids() { return poids; }

    /**
     * Retourne le type de la route.
     * @return - Type de la route.
     */
    public String getTypeRoute() { return typeRoute; }

    /**
     * Retourne l'identifiant du camion qui emprunte cette route.
     * @return - Identifiant du camion.
     */
    public int getCamionId() { return camionId; }
}