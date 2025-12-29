package src.ui;

import src.modeles.Graphe;
import src.ClientThread;
import src.modeles.Vertex;
import src.modeles.Edge;
import src.Utils;
import org.json.JSONArray;
import org.json.JSONObject;
import javax.swing.*;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.LineBorder;
import javax.swing.border.MatteBorder;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class ControlPanel extends JPanel {

    private JComboBox<String> regionComboBox;
    private JComboBox<String> modeComboBox;
    private JComboBox<String> departComboBox;
    private JSpinner camionsSpinner;
    private JButton loadButton;
    private JButton lancerButton;
    private JButton showLastButton;

    private final GraphPanel graphPanel;
    private Graphe lastResult = null;

    // --- Composants UI ---
    private JPanel contentPanel;
    private JButton toggleButton;

    // --- PALETTE SOBRE ---
    private final Color BG_COLOR = Color.WHITE;
    private final Color HEADER_BG = new Color(30, 30, 30); // Noir
    private final Color HEADER_TEXT = Color.WHITE;

    // Couleurs Unifiées pour les actions (Style "Charger la carte")
    private final Color BTN_BG = Color.WHITE;
    private final Color BTN_FG = Color.BLACK;

    // Couleur Désactivé (Gris béton bien visible)
    private final Color BTN_DISABLED_BG = new Color(220, 220, 220);
    private final Color BTN_DISABLED_FG = new Color(120, 120, 120);

    private final Font FONT_UI = new Font("Segoe UI", Font.PLAIN, 13);
    private final Font FONT_BOLD = new Font("Segoe UI", Font.BOLD, 13);

    public ControlPanel(GraphPanel graphPanel) {
        this.graphPanel = graphPanel;

        setLayout(new BorderLayout());
        setBackground(BG_COLOR);
        setBorder(new MatteBorder(0, 1, 0, 0, Color.BLACK));

        // 1. HEADER
        JPanel headerPanel = new JPanel(new BorderLayout());
        headerPanel.setBackground(HEADER_BG);
        headerPanel.setBorder(new EmptyBorder(12, 15, 12, 15));
        headerPanel.setPreferredSize(new Dimension(320, 50));

        JLabel titleLabel = new JLabel("CONFIGURATION");
        titleLabel.setFont(new Font("Segoe UI", Font.BOLD, 14));
        titleLabel.setForeground(HEADER_TEXT);

        toggleButton = new JButton("▼");
        styleHeaderButton(toggleButton);
        toggleButton.addActionListener(e -> toggleContent());

        headerPanel.add(titleLabel, BorderLayout.WEST);
        headerPanel.add(toggleButton, BorderLayout.EAST);
        add(headerPanel, BorderLayout.NORTH);

        // 2. CONTENU
        contentPanel = new JPanel(new GridBagLayout());
        contentPanel.setBackground(BG_COLOR);
        contentPanel.setBorder(new EmptyBorder(20, 20, 20, 20));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 0, 8, 0);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1.0;
        gbc.gridx = 0;

        // --- BLOC 1 : CARTE ---
        addSectionTitle(contentPanel, "1. CHOIX DE LA CARTE", gbc);

        List<String> regions = RegionUtils.getRegionNames();
        regionComboBox = new JComboBox<>(regions.toArray(new String[0]));
        styleCombo(regionComboBox);
        contentPanel.add(regionComboBox, nextGbc(gbc));

        loadButton = new JButton("CHARGER LA CARTE");
        styleActionButton(loadButton); // Style unifié
        contentPanel.add(loadButton, nextGbc(gbc));

        showLastButton = new JButton("Revoir le dernier résultat");
        styleLinkButton(showLastButton);
        showLastButton.setEnabled(false);
        contentPanel.add(showLastButton, nextGbc(gbc));

        addSeparator(contentPanel, gbc);

        // --- BLOC 2 : MISSION ---
        addSectionTitle(contentPanel, "2. PARAMÈTRES MISSION", gbc);

        contentPanel.add(createLabel("Mode de calcul :"), nextGbc(gbc));
        String[] modes = {"DISTANCE", "TEMPS"};
        modeComboBox = new JComboBox<>(modes);
        styleCombo(modeComboBox);
        contentPanel.add(modeComboBox, nextGbc(gbc));

        contentPanel.add(createLabel("Ville de départ :"), nextGbc(gbc));
        departComboBox = new JComboBox<>();
        styleCombo(departComboBox);
        departComboBox.setEnabled(false);
        contentPanel.add(departComboBox, nextGbc(gbc));

        contentPanel.add(createLabel("Nombre de camions :"), nextGbc(gbc));
        camionsSpinner = new JSpinner(new SpinnerNumberModel(1, 1, 10, 1));
        JComponent editor = camionsSpinner.getEditor();
        if (editor instanceof JSpinner.DefaultEditor) {
            ((JSpinner.DefaultEditor)editor).getTextField().setFont(FONT_UI);
        }
        contentPanel.add(camionsSpinner, nextGbc(gbc));

        addSeparator(contentPanel, gbc);

        lancerButton = new JButton("LANCER LE CALCUL");
        styleActionButton(lancerButton); // MÊME STYLE QUE CHARGER (Blanc/Noir)
        lancerButton.setEnabled(false);

        gbc.insets = new Insets(25, 0, 0, 0);
        contentPanel.add(lancerButton, nextGbc(gbc));

        // Push vers le haut
        gbc.weighty = 1.0;
        contentPanel.add(Box.createVerticalGlue(), nextGbc(gbc));

        JScrollPane scrollPane = new JScrollPane(contentPanel);
        scrollPane.setBorder(null);
        add(scrollPane, BorderLayout.CENTER);

        // --- Init Logique ---
        initLogic();
    }

    // --- STYLING UNIFIÉ ---

    private void styleActionButton(JButton btn) {
        btn.setFont(FONT_BOLD);
        btn.setFocusPainted(false);
        btn.setCursor(new Cursor(Cursor.HAND_CURSOR));

        // Listener pour changer l'apparence dynamique (Gris quand désactivé, Blanc/Noir quand activé)
        btn.addPropertyChangeListener("enabled", evt -> updateButtonLook(btn));

        // Appel initial
        updateButtonLook(btn);
    }

    private void updateButtonLook(JButton btn) {
        if (btn.isEnabled()) {
            // STYLE ACTIVÉ (Pareil pour Charger et Lancer)
            btn.setBackground(BTN_BG);      // Blanc
            btn.setForeground(BTN_FG);      // Noir
            // Bordure Noire Épaisse
            btn.setBorder(new CompoundBorder(
                    new LineBorder(Color.BLACK, 2),
                    new EmptyBorder(10, 15, 10, 15)
            ));
        } else {
            // STYLE DÉSACTIVÉ (Gris Béton)
            btn.setBackground(BTN_DISABLED_BG);
            btn.setForeground(BTN_DISABLED_FG);
            // Bordure Grise
            btn.setBorder(new CompoundBorder(
                    new LineBorder(Color.GRAY, 2),
                    new EmptyBorder(10, 15, 10, 15)
            ));
        }
    }

    private void styleHeaderButton(JButton btn) {
        btn.setFocusPainted(false);
        btn.setContentAreaFilled(false);
        btn.setBorderPainted(false);
        btn.setFont(new Font("Segoe UI", Font.BOLD, 16));
        btn.setForeground(Color.WHITE);
        btn.setCursor(new Cursor(Cursor.HAND_CURSOR));
    }

    private void styleLinkButton(JButton btn) {
        btn.setFocusPainted(false);
        btn.setContentAreaFilled(false);
        btn.setBorderPainted(false);
        btn.setFont(new Font("Segoe UI", Font.ITALIC, 12));
        btn.setForeground(Color.BLUE.darker());
        btn.setCursor(new Cursor(Cursor.HAND_CURSOR));
    }

    private void styleCombo(JComboBox box) {
        box.setBackground(Color.WHITE);
        box.setFont(FONT_UI);
    }

    private JLabel createLabel(String text) {
        JLabel l = new JLabel(text);
        l.setFont(FONT_UI);
        l.setForeground(Color.BLACK);
        return l;
    }

    private void addSectionTitle(JPanel p, String text, GridBagConstraints gbc) {
        JLabel l = new JLabel(text);
        l.setFont(new Font("Segoe UI", Font.BOLD, 11));
        l.setForeground(Color.BLACK);
        l.setBorder(new MatteBorder(0, 0, 2, 0, Color.BLACK));
        gbc.gridy++;
        gbc.insets = new Insets(15, 0, 10, 0);
        p.add(l, gbc);
        gbc.insets = new Insets(5, 0, 5, 0);
    }

    private void addSeparator(JPanel p, GridBagConstraints gbc) {
        JSeparator sep = new JSeparator();
        sep.setForeground(Color.LIGHT_GRAY);
        gbc.gridy++;
        gbc.insets = new Insets(15, 0, 15, 0);
        p.add(sep, gbc);
        gbc.insets = new Insets(5, 0, 5, 0);
    }

    private GridBagConstraints nextGbc(GridBagConstraints gbc) {
        gbc.gridy++;
        return gbc;
    }

    private void toggleContent() {
        boolean isVisible = contentPanel.isVisible();
        contentPanel.setVisible(!isVisible);
        toggleButton.setText(!isVisible ? "▼" : "◀");
        revalidate();
        repaint();
    }

    // --- LOGIQUE MÉTIER ---
    private void initLogic() {
        loadButton.addActionListener(e -> {
            String selectedRegion = (String) regionComboBox.getSelectedItem();
            if (selectedRegion != null) {
                String filePath = RegionUtils.getRegionFilePath(selectedRegion);
                loadRegionData(selectedRegion, filePath);
            }
        });

        showLastButton.addActionListener(e -> {
            if (lastResult != null) graphPanel.setGraphe(lastResult);
        });

        lancerButton.addActionListener(e -> {
            String region = (String) regionComboBox.getSelectedItem();
            String mode = (String) modeComboBox.getSelectedItem();
            String villeDepart = (String) departComboBox.getSelectedItem();
            int nbCamions = (Integer) camionsSpinner.getValue();
            String commande = region + ";" + mode + ";" + villeDepart + ";" + nbCamions;

            System.out.println("Envoi commande : " + commande);

            new Thread(() -> {
                ClientThread client = new ClientThread(1, commande) {
                    public void onResult(Graphe data, String label) {
                        if (data != null) {
                            SwingUtilities.invokeLater(() -> {
                                lastResult = data;
                                showLastButton.setEnabled(true);
                                graphPanel.setGraphe(data);
                            });
                        }
                    }
                };
                client.start();
            }).start();
        });
    }

    private void loadRegionData(String regionName, String filePath) {
        try (java.io.BufferedReader reader = new java.io.BufferedReader(new java.io.FileReader(filePath))) {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) sb.append(line);

            String content = sb.toString().trim();
            Graphe graphe = null;

            if (content.startsWith("[")) {
                JSONArray array = new JSONArray(content);
                List<Vertex> vertices = new ArrayList<>();
                for (int i = 0; i < array.length(); i++) {
                    JSONObject obj = array.getJSONObject(i);
                    vertices.add(new Vertex(obj.getString("ville"), obj.getDouble("latitude"), obj.getDouble("longitude")));
                }
                graphe = new Graphe(regionName, vertices, new ArrayList<>());
            } else {
                try (java.io.BufferedReader br = new java.io.BufferedReader(new java.io.StringReader(content))) {
                    graphe = Utils.parseJson(br);
                }
            }

            if (graphe != null) {
                graphPanel.setGraphe(graphe);
                departComboBox.removeAllItems();
                for (Vertex v : graphe.getVertices()) departComboBox.addItem(v.getName());
                departComboBox.setEnabled(true);
                lancerButton.setEnabled(true);
            } else {
                resetUIOnError();
            }
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "Erreur chargement : " + ex.getMessage());
            resetUIOnError();
        }
    }

    private void resetUIOnError() {
        departComboBox.removeAllItems();
        departComboBox.setEnabled(false);
        lancerButton.setEnabled(false);
    }
}