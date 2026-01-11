package src.tests;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import src.modeles.*;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

class GrapheTest {

    private Graphe graphe;
    private Sommet s1, s2, s3;
    private Arete a1, a2, a3;

    @BeforeEach
    void setUp() {
        // Initialisation des sommets (coordonnées monde)
        s1 = new Sommet("Ville A", 48.8566, 2.3522);
        s2 = new Sommet("Ville B", 45.7640, 4.8357);
        s3 = new Sommet("Ville C", 43.2965, 5.3698);
        List<Sommet> sommets = Arrays.asList(s1, s2, s3);

        // Initialisation des arêtes (Données passives)
        a1 = new Arete(s1, s2, 130, "Autoroute", 1);
        a2 = new Arete(s2, s3, 80, "Départementale", 1);
        a3 = new Arete(s1, s3, 50, "Nationale", 2);

        List<Arete> aretes = Arrays.asList(a1, a2, a3);

        // Création du graphe
        graphe = new Graphe("Test Graphe", sommets, aretes);
    }

    @Test
    void testGetSommetsEtAretes() {
        assertEquals(3, graphe.getSommets().size());
        assertEquals(3, graphe.getAretes().size());
    }

    @Test
    void testDistanceTotale() {
        // On simule la réception d'une valeur calculée par le serveur C++
        double distanceSimulee = 260.0;
        graphe.setDistanceTotale(distanceSimulee);

        // On vérifie que le getter (sans argument) renvoie bien la valeur stockée
        assertEquals(260.0, graphe.getDistanceTotale(), 0.001);
    }

    @Test
    void testDureeTotale() {
        // On simule la réception d'une durée calculée par le serveur C++
        double dureeSimulee = 3.5;
        graphe.setDureeTotale(dureeSimulee);

        assertEquals(3.5, graphe.getDureeTotale(), 0.001);
    }

    @Test
    void testFiltrageParCamion() {
        // Ce test vérifie une fonction utilitaire d'affichage (autorisée en Java)
        List<Arete> aretesCamion2 = graphe.getAretesParCamion(2);

        assertEquals(1, aretesCamion2.size());
        assertEquals("Nationale", aretesCamion2.get(0).getTypeRoute());
        assertEquals(50, aretesCamion2.get(0).getPoids());
    }

    @Test
    void testDureeErreur() {
        // On teste le comportement en cas d'erreur de calcul serveur (valeur négative)
        graphe.setDureeTotale(-1.0);
        assertTrue(graphe.getDureeTotale() < 0);
    }
}