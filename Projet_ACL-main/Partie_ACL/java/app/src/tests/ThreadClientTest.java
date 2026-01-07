import org.junit.jupiter.api.Test;
import src.ThreadClient;
import src.modeles.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.*;

class ThreadClientTest {

	private Graphe grapheRecu = null;

	@Test
	void testRunConnexionEtTraitement() throws InterruptedException {
		// Le verrou pour attendre que le thread termine
		CountDownLatch latch = new CountDownLatch(1);
		String requeteTest = "Nancy;Metz";

		// 1. Création d'un serveur Mock sur le port 8080
		Thread serveurSimule = new Thread(() -> {
			try (ServerSocket serverSocket = new ServerSocket(8080)) {
				// On accepte la connexion du client
				try (Socket clientSocket = serverSocket.accept();
					 BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
					 PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {

					// On lit ce que le client envoie pour valider
					in.readLine();

					// On envoie le JSON que UtilitaireCalcul attend
					// Note : J'utilise "vertices" et "edges" car ton erreur précédente les demandait
					String json = "{ \"windowName\": \"TestRegion\", \"vertices\": [], \"edges\": [] }";
					out.println(json);
					out.flush();
				}
			} catch (IOException e) {
				System.err.println("Erreur Serveur Mock : " + e.getMessage());
			}
		});

		serveurSimule.setDaemon(true); // Pour ne pas bloquer la JVM si le test échoue
		serveurSimule.start();

		// 2. Initialisation du client
		ThreadClient client = new ThreadClient(requeteTest) {
			@Override
			public void traiterResultat(Graphe graphe, String etiquette) {
				grapheRecu = graphe;
				latch.countDown(); // On libère le verrou quand le résultat est traité
			}
		};

		// 3. Lancement du client
		client.start();

		// 4. Attente du résultat (max 5 secondes)
		// C'est ici que le test attendait dans le vide avant
		boolean succes = latch.await(5, TimeUnit.SECONDS);

		// 5. Vérifications
		assertTrue(succes, "Le serveur n'a pas répondu ou le format JSON était incorrect.");
		assertNotNull(grapheRecu, "Le graphe ne doit pas être nul.");
		assertEquals("TestRegion", grapheRecu.getWindowName());
	}
}