# EVE – Neuronaler Handlungskontext

## Zweck und Geltungsbereich

Dieses Dokument spezifiziert den in Forschungsstufe B.3.1 untersuchten Mechanismus. Er verbindet tatsächlich auftretende neuronale Handlungsaktivität zeitlich begrenzt mit lokalen Eligibility-Entscheidungen. Er führt noch keine Gewichtsänderung aus.

Der Mechanismus ist ausdrücklich keine globale Handlungsverwaltung. Das BIOS darf für die unabhängige Versuchsauswertung Episoden nummerieren; diese Nummer wird weder in Ereignisse noch in Kontextspuren, Eligibility oder Rückmeldungen geschrieben.

## Informationsfluss

Eine Kontextquelle aktiviert reguläre Eingangsneuronen. Die dabei aktive Eingangsmenge wird deterministisch zu einer kompakten Signatur abgebildet. Diese Signatur besitzt keine semantische Bedeutung und ist nicht eindeutig pro Handlung:

- dieselbe Quellaktivität erzeugt dieselbe Signatur,
- identische motorische Ausgaben bleiben deshalb ununterscheidbar,
- eine stille motorische Ausgabe erzeugt in C2 keine künstliche Signatur,
- die Signatur enthält weder Episodennummer noch Zielwert oder Bewertung.

Die Signatur begleitet ausschließlich die aus diesem Feuerereignis entstehenden regulären Impulse. Empfangende Neuronen können daraus eine lokale, ablaufende Kontextspur bilden. Die technische Rückmeldung erhält die Signatur der bewerteten Handlung nicht.

## Kontextquellen C0 bis C4

| Variante | Quellaktivität | Angeborene Kontextverschaltung |
|---|---|---|
| C0 | keine | keine |
| C1 | allgemeines Start-/Endsignal | keine zusätzliche Verteilung |
| C2 | Kopie tatsächlich aktivierter Motorbits | keine zusätzliche Verteilung |
| C3 | Start-/Endsignal | vier allgemeine Relais mit begrenztem Fan-out |
| C4 | Start-/Endsignal und tatsächliche Motorbits | vier allgemeine Relais mit begrenztem Fan-out |

C1 und C2 prüfen reine Quellen. C3 prüft die gemeinsame Verteilung eines allgemeinen Grenzsignals. C4 kombiniert allgemeine Grenzen mit motorischer Eigenaktivität. Die Verschaltungen werden deterministisch aus Seed, reservierten Bereichen und allgemeinen Topologieregeln gebildet. Sie kennen weder M3-Kausalitätsdiagnose noch technische Handlungsteilnehmer.

## Lokale Kontextspuren P0 bis P2

Eine Kontextspur enthält:

- eine nur im jeweiligen Neuron fortlaufende Generation,
- die aus realer Quellaktivität abgeleitete Signatur,
- den Ablaufzeitpunkt,
- die Herkunft `regulär` oder `rückmeldungsinduziert`.

Varianten:

- P0: keine Kontextspur, Kapazität 0,
- P1: genau ein fester Platz pro Neuron,
- P2: zwei feste Plätze pro Neuron.

Die Speicherung ist ein begrenzter Ring. Es gibt keine dynamisch wachsende Teilnehmerliste, keinen Zeitstempel pro Synapse und keinen vollständigen Netzwerkscan. Jede Spur erhält beim Anlegen ein eigenes Ablaufereignis. Ein überholtes Ablaufereignis wird als veraltet gezählt und verändert keinen neueren Zustand.

Kontextspuren sind getrennt von Membranpotenzial, Refraktärzustand, G3 und Eligibility. Ihr Ablauf feuert kein Neuron, verändert keine Synapse und setzt keine G3-Aktivität zurück.

## Bindung an Eligibility

Eine Eligibility-Entscheidung kann zusätzlich folgende lokale Kontextdaten tragen:

- Signatur und lokale Kontextgeneration,
- Mehrdeutigkeitskennzeichen,
- Herkunft der gebundenen Kontextspur.

Die Kandidatensynapsen und ihre bisherige Auswahl L1/L2 bleiben unverändert.

| Variante | Regel |
|---|---|
| B0 | Eligibility bleibt ohne Kontextbezug. |
| B1 | Beim Feuerentscheid wird die jüngste lokal aktive Spur gebunden. Unterschiedliche gleichzeitig aktive Signaturen markieren den Bezug als mehrdeutig. |
| B2 | Eine später lokal eintreffende Kontextspur darf eine noch ungebundene Eligibility binden. Eine abweichende weitere Spur ersetzt den Bezug nicht, sondern markiert Mehrdeutigkeit. |

B2 verwendet ausschließlich lokale zeitliche Überlappung. Es findet keine nachträgliche Rekonstruktion über technische M3-Daten statt.

## Lokale Rückmeldungsdiagnose

Vor der Verarbeitung jedes Rückmeldungsimpulses werden nur bereits vorhandene Eligibility-Einträge regulärer Herkunft betrachtet. Durch die Rückmeldung neu erzeugte Eligibility zählt nicht rückwirkend zur Handlung.

Die lokale Diagnose unterscheidet:

- keine vorhandene Eligibility,
- Eligibility ohne gebundenen Kontext,
- genau eine aktive passende Signatur,
- mehrere oder als mehrdeutig markierte passende Signaturen,
- gebundene, aber bereits abgelaufene Signatur,
- Bindung an rückmeldungsinduzierten Kontext.

„Passend“ bedeutet hier nur: gebundene Signatur und lokal noch aktive Kontextspur stimmen überein. Der neuronale Kern entscheidet nicht, zu welcher technischen Episode diese Signatur gehört.

## Unabhängige Wahrheitsgrundlage

Der B.3.1-Versuchsläufer führt außerhalb des neuronalen Kerns technische Episodennummern. Ein Beobachter liest nur diejenigen Signaturen, die an einem Rückmeldungsempfänger zugleich in regulärer Eligibility und aktiver lokaler Kontextspur vorkommen.

Die Zuordnungsqualität wird anschließend bestimmt:

- `correct`: genau eine technische Episode ist mit den beobachteten Signaturen vereinbar und sie ist die bewertete Episode,
- `false`: genau eine andere Episode ist vereinbar,
- `ambiguous`: mehrere Episoden sind vereinbar,
- `unassigned`: keine Episode ist vereinbar.

Diese Diagnose beeinflusst die Simulation nicht. Identische Episoden mit derselben neuronalen Information bleiben mehrdeutig; sie werden nicht durch ihre technische Nummer auseinanderdividiert.

## Ereignis- und Persistenzmodell

Reguläre Feuer- und Impulsereignisse können die Kontextsignatur und deren Herkunft tragen. Das zusätzliche Ereignis `context_expiry` entfernt genau eine lokale Generation. Format 8 speichert:

- Kontextkonfiguration,
- alle aktiven P1/P2-Spuren,
- Kontextbezüge der Eligibility,
- neue Metriken,
- noch ausstehende Kontextabläufe,
- Kontextmetadaten noch ausstehender regulärer Ereignisse.

Formate 1 bis 7 bleiben lesbar. Für sie wird der Kontextmechanismus deaktiviert und der fehlende Zustand leer rekonstruiert.

## Ressourcenbegrenzung

Der Kontextzustand ist proportional zu `Neuronen × Kontextplätze`. P2 belegt in der gemessenen C++-Darstellung zwei `ContextTrace`-Einträge pro Neuron. Die Rohschätzung beträgt:

- 256 Neuronen: 16.384 Byte,
- 10.000 Neuronen: 640.000 Byte,
- 100.000 Neuronen: 6.400.000 Byte,
- 1.000.000 Neuronen: 64.000.000 Byte.

Zusätzlich entstehen begrenzte Ablaufereignisse. Überschreiben und veraltete Abläufe werden explizit gemessen.

## Forschungsgrenzen

Der Mechanismus implementiert nicht:

- positive oder negative rückmeldungsabhängige Gewichtsänderungen,
- synaptische Löschung, Hemmung oder STDP,
- eine Erfolgserwartung oder Vorhersagefehlerregel,
- einen globalen Kontext- oder Handlungszähler,
- eine technische Rückwärtsmarkierung des M3-Pfades,
- eine semantische Bedeutung einzelner Kontextsignaturen.

Alle B.3.1-Läufe halten Lernen und Wachstum deaktiviert. Die Gewichtsinvarianz ist Teil jedes Rohdatensatzes.
