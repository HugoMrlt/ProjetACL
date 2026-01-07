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
		// Initialisation des sommets
		s1 = new Sommet("Ville A", 0, 0);
		s2 = new Sommet("Ville B", 10, 0);
		s3 = new Sommet("Ville C", 10, 10);
		List<Sommet> sommets = Arrays.asList(s1, s2, s3);

		// Initialisation des arêtes
		// Supposons : Autoroute (130km/h), Départementale (80km/h)
		a1 = new Arete(s1, s2, 130, "Autoroute", 1);
		a2 = new Arete(s2, s3, 80, "Départementale", 1);
		a3 = new Arete(s1, s3, 50, "Ville", 2); // Camion différent

		List<Arete> aretes = Arrays.asList(a1, a2, a3);

		graphe = new Graphe("Carte Test", sommets, aretes);
	}

	@Test
	void testGetDistanceTotaleToutesAretes() {
		// 130 + 80 + 50 = 260
		assertEquals(260.0, graphe.getDistanceTotale(-1), 0.001);
	}

	@Test
	void testGetDistanceTotaleParCamion() {
		// Camion 1 : 130 + 80 = 210
		assertEquals(210.0, graphe.getDistanceTotale(1), 0.001);
		// Camion 2 : 50
		assertEquals(50.0, graphe.getDistanceTotale(2), 0.001);
	}

	@Test
	void testGetDistanceTotaleCamionInexistant() {
		assertTrue(Double.isNaN(graphe.getDistanceTotale(99)));
	}

	@Test
	void testGetDureeTotale() {
		// Camion 1 :
		// a1: 130km / 130km/h = 1h
		// a2: 80km / 80km/h = 1h
		// Total = 2h
		assertEquals(2.0, graphe.getDureeTotale(1), 0.001);
	}

	@Test
	void testGetDureeTotaleGrapheComplet() {
		// On suppose que "Ville" dans TypeRoute a une vitesse définie (ex: 50km/h)
		// Camion 1 (2h) + Camion 2 (50km / 50km/h = 1h) = 3h
		assertEquals(3.0, graphe.getDureeTotale(-1), 0.001);
	}

	@Test
	void testGetDureeTotaleAvecRouteInconnue() {
		// Ajout d'une arête avec un type inexistant ou vitesse <= 0
		Arete aInconnue = new Arete(s1, s2, 100, "RouteInexistante", 3);
		List<Arete> nouvellesAretes = new ArrayList<>(graphe.getAretes());
		nouvellesAretes.add(aInconnue);

		Graphe grapheInconnu = new Graphe("Test Inconnu", graphe.getSommets(), nouvellesAretes);

		// La durée pour le camion 3 devrait être NaN car la vitesse est inconnue
		assertTrue(Double.isNaN(grapheInconnu.getDureeTotale(3)));

		// La durée totale du graphe ne devrait pas changer car l'arête inconnue est ignorée
		assertEquals(3.0, grapheInconnu.getDureeTotale(-1), 0.001);
	}

	@Test
	void testGetters() {
		assertEquals("Carte Test", graphe.getWindowName());
		assertEquals(3, graphe.getSommets().size());
		assertEquals(3, graphe.getAretes().size());
	}
}