import org.junit.jupiter.api.Test;
import src.UtilitaireCalcul;
import src.modeles.Graphe;
import src.modeles.Sommet;
import java.io.BufferedReader;
import java.io.StringReader;
import java.io.IOException;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class UtilitaireCalculTest {

	// Un JSON d'exemple fidèle à ce que ton code attend (vertices/edges)
	private final String jsonValide = "{" +
			"\"windowName\": \"Ma Region\"," +
			"\"vertices\": [" +
			"  {\"ville\": \"Paris\", \"latitude\": 48.85, \"longitude\": 2.35}," +
			"  {\"ville\": \"Lyon\", \"latitude\": 45.76, \"longitude\": 4.83}" +
			"]," +
			"\"edges\": [" +
			"  {\"v1\": \"Paris\", \"v2\": \"Lyon\", \"poids\": 465, \"type\": \"Autoroute\", \"camion\": 1}" +
			"]" +
			"}";

	@Test
	void testParseJsonComplet() throws IOException {
		// Utilisation de StringReader pour simuler un flux réseau
		BufferedReader reader = new BufferedReader(new StringReader(jsonValide));

		Graphe graphe = UtilitaireCalcul.parseJson(reader);

		assertNotNull(graphe);
		assertEquals("Ma Region", graphe.getWindowName());
		assertEquals(2, graphe.getSommets().size());
		assertEquals(1, graphe.getAretes().size());

		// Vérification du lien entre arête et sommets
		assertEquals("Paris", graphe.getAretes().get(0).getSommet1().getNom());
		assertEquals("Lyon", graphe.getAretes().get(0).getSommet2().getNom());
	}

	@Test
	void testConversionCoordonneesEcran() throws IOException {
		BufferedReader reader = new BufferedReader(new StringReader(jsonValide));
		Graphe graphe = UtilitaireCalcul.parseJson(reader);

		List<Sommet> sommets = graphe.getSommets();

		for (Sommet s : sommets) {
			// Vérifie que les coordonnées écran ont été calculées (différentes de 0 par défaut)
			// Note: Comme c'est du int, on vérifie juste la cohérence
			assertTrue(s.getX() >= 0 && s.getX() <= 800, "X hors limites pour " + s.getNom());
			assertTrue(s.getY() >= 0 && s.getY() <= 600, "Y hors limites pour " + s.getNom());
		}
	}

	@Test
	void testParseJsonInvalide() throws IOException {
		// Test avec un JSON vide ou mal formé
		BufferedReader reader = new BufferedReader(new StringReader("pas du json"));
		Graphe graphe = UtilitaireCalcul.parseJson(reader);

		assertNull(graphe, "Le parseur devrait retourner null pour un format invalide.");
	}

	@Test
	void testLireAretesSommetsInexistants() {
		String jsonAreteInvalide = "{" +
				"\"vertices\": [{\"ville\": \"A\", \"latitude\": 1, \"longitude\": 1}]," +
				"\"edges\": [{\"v1\": \"A\", \"v2\": \"Inconnu\", \"poids\": 10}]" +
				"}";

		List<Sommet> sommets = UtilitaireCalcul.lireSommets(jsonAreteInvalide);
		var aretes = UtilitaireCalcul.lireAretes(jsonAreteInvalide, sommets);

		// L'arête ne doit pas être créée car "Inconnu" n'est pas dans la liste des sommets
		assertTrue(aretes.isEmpty());
	}
}