# EVE – Forschungsbericht Forschungsstufe B.3.1

## Neuronaler Handlungskontext und zuverlässige Zuordnung verzögerter Rückmeldungen

**Stand:** 18. November 2026  
**Ausgangsbasis:** `codex/research-stage-b3-neural-feedback-routing`  
**Versuchsläufer:** `eve_research_stage_b31`  
**Seeds:** 64, davon 6 zusätzlich im fortlaufenden Betrieb  
**Regression:** 173/173 Tests bestanden  
**Gewichtslernen und Wachstum:** deaktiviert

## Kurzfazit

EVE kann aus tatsächlichen Grenz- und Motorereignissen lokale, ablaufende neuronale Kontextspuren bilden, sie über allgemeine Verschaltungen verteilen und an lokale Eligibility binden. Die Rückmeldung erreicht solche gebundenen Zustände. Damit sind Kontextbildung, -verteilung und lokale Bindung technisch und empirisch nachgewiesen.

Die Zuordnung mehrerer Handlungen ist jedoch nicht zuverlässig genug für B.4. Im Zwei-Handlungs-Versuch waren nur 96 von 2.688 Rückmeldungen eindeutig richtig zugeordnet (3,57 %). 18 waren eindeutig falsch (0,67 %), 1.447 mehrdeutig (53,83 %) und 1.127 nicht zuordenbar (41,93 %). Identische Handlungen bleiben ohne zusätzliche neuronale Information absichtlich ununterscheidbar. Bei vertauschter Rückmeldungsreihenfolge traten 18 eindeutige Fehlzuordnungen auf.

Der Befund ist deshalb ein begründetes **Nein zum jetzigen Übergang zu B.4**. Eine Lernregel würde derzeit überwiegend mehrdeutige, fehlende oder in vertauschten Folgen falsche Handlungsbezüge verstärken.

## A. Ausgangssituation und Methode

B.3 wies die neuronale Zustellung abgestufter Bewertung nach, konnte die bewertete Handlung aber nicht bestimmen. Die damalige statische Funktion `run_assignment()` wurde in B.3.1 nicht fortgeführt. Alle Zuordnungszeilen stammen aus tatsächlich ausgeführten neuronalen Läufen.

Der Versuchsaufbau behielt die etablierte Basis bei:

- 256 Neuronen und 1.024 reguläre Startsynapsen,
- V2 mit Gewicht 3,
- M3 mit Gewichten 1 und 3,
- Feuergrenze 3,
- G3 mit 8 Einträgen,
- R2F mit Dauer 16,
- L2 mit 2 Kandidaten, 2 Entscheidungen und Dauer 64,
- F2/F3-Rückmeldung,
- E4-neu, Lernen und Wachstum aus.

L1, R1 und vollständig deaktivierte Varianten wurden als Kontrollen ausgeführt. Die maschinenlesbaren Ergebnisse enthalten 5.892 echte Versuchs- und Messzeilen zu Kontextvarianten, Handlungsfolgen, K0–K15, Dauerbetrieb, Persistenz und Ressourcen.

### Unabhängige Wahrheitsgrundlage

Der neuronale Kern kennt keine Handlungs-ID. Nur der Versuchsläufer nummeriert Episoden. Ein Beobachter erfasst bei Rückmeldungsimpulsen lokal gleichzeitig aktive Kontextspuren und regulär entstandene Eligibility. Erst danach vergleicht die Forschungsdiagnose diese Signaturen mit den Episoden:

- genau die beabsichtigte Episode: `correct`,
- genau eine andere Episode: `false`,
- mehrere Episoden: `ambiguous`,
- keine Episode: `unassigned`.

Technische Episodennummern werden nicht in Ereignisse, Kontextspuren, Eligibility oder Rückmeldungen geschrieben.

## B. Tatsächliche Ereignisfolge und Kontextquellen

Der beobachtete Ablauf war:

1. neuronales Startgrenzsignal, sofern die Variante es zulässt,
2. sensorische Byteaktivität,
3. V2- und explorative Aktivität,
4. motorische Daten- und M3-Aktivität,
5. motorischer Abschluss oder Ende des Sicherheitsfensters,
6. Kontext aus tatsächlich beobachteten Motorbits, sofern vorhanden,
7. neuronales Endgrenzsignal,
8. später eintreffende F2/F3-Bewertung,
9. unabhängige Erholungs-, Eligibility- und Kontextabläufe.

Grenzsignale sind zuverlässig vorhanden, unterscheiden aber einzelne Handlungen nicht. Motorische Eigenaktivität kann Ausgaben unterscheiden, ist bei identischer Ausgabe identisch und fehlt bei einer stillen Nullausgabe vollständig. Ein allgemeiner Kontextkreis verteilt Informationen breiter, erhöht aber gleichzeitig Überlappung und Mehrdeutigkeit.

## C. Technische Umsetzung

Implementiert wurden:

- C0 bis C4 als echte Quellen und reguläre neuronale Verschaltungen,
- P0, P1 und P2 als feste lokale SoA-Zustände,
- ereignisgesteuerter Ablauf ohne Netzwerkscan,
- B0, B1 und B2,
- Herkunftstrennung für regulären und rückmeldungsinduzierten Kontext,
- lokale Diagnose für eindeutig, mehrfach, fehlend und abgelaufen,
- Persistenzformat 8 mit Rückwärtslesbarkeit der Formate 1–7,
- 32 neue automatisierte B.3.1-Prüfungen.

Eine Kontextsignatur ist ein stabiler Fingerabdruck der tatsächlich aktiven Quellneuronen. Sie ist keine Episodennummer: Zwei gleiche Motoraktivitäten erzeugen dieselbe Signatur. Die Signatur hat keine vorgegebene Bedeutung und die Rückmeldung trägt die Signatur der bewerteten Episode nicht.

## D. Vergleich C0 bis C4

Die Kontextmatrix enthält 1.088 Läufe. In jedem Lauf wurde genau eine Episode ausgeführt; daher kann eine gefundene Signatur noch nicht mit einer anderen Episode konkurrieren.

| Variante | Spur/Bindung | richtig | nicht zugeordnet | erzeugte Spuren | Bindungen |
|---|---:|---:|---:|---:|---:|
| C0 | P0/B0 | 0/64 | 64/64 | 0 | 0 |
| C1 | P2/B1 | 3/64 | 61/64 | 725 | 46 |
| C2 | P2/B1 | 0/64 | 64/64 | 317 | 5 |
| C2 | P2/B2 | 3/64 | 61/64 | 317 | 23 |
| C3 | P1/B1 | 40/64 | 24/64 | 4.844 | 870 |
| C3 | P2/B1 | 42/64 | 22/64 | 4.844 | 870 |
| C4 | P1/B1 | 39/64 | 25/64 | 5.673 | 966 |
| C4 | P2/B1 | 41/64 | 23/64 | 5.673 | 966 |
| C4 | P2/B2 | **43/64** | 21/64 | 5.673 | 966 |

Über alle 1.088 Läufe ergaben sich 259 richtige und 829 nicht zugeordnete Einzelaktionen. Ohne Bindung B0 waren alle Varianten nicht zugeordnet. C1/C2 erzeugten lokal reale Spuren, erreichten aber zu selten dieselben Orte wie Eligibility und Rückmeldung. C3/C4 erhöhten die Abdeckung deutlich. C4/P2/B2 erreichte mit 43/64 die beste Einzelaktionsquote.

Die 239 abgeschlossenen Motorantworten in der Gesamtmatrix zeigen zugleich, dass ein neuronaler Episodenkontext nicht mit einer erfolgreich abgeschlossenen Handlung gleichgesetzt werden darf. Grenzkontext kann auch um eine unvollständige Handlung entstehen.

## E. Verbindung mit Eligibility

B1/B2 banden in der Kontextmatrix 5.624 Eligibility-Entscheidungen; 206 Bindungen wurden bereits beim Anlegen als mehrdeutig erkannt. B2 konnte später lokal eintreffende Spuren binden, war aber nicht pauschal besser als B1.

Wesentliche Negativbefunde:

- C1/P2/B2 band nur 29 Entscheidungen über 64 Seeds,
- C2/P2/B2 band 23 Entscheidungen,
- C3/C4 erzeugten eine hohe Abdeckung, aber auch viele gleichzeitig aktive Spuren,
- B0 lieferte trotz tausender Kontextspuren keine technische Zuordnung.

Damit ist nicht die Existenz einer Spur, sondern ihre lokale Koinzidenz mit Eligibility und späterer Rückmeldung der Engpass.

## F. Rückmeldungszuordnung über zwei Handlungen

Die 2.688 Läufe umfassen sechs Folgen und R0 bis R6.

| Ergebnis | Anzahl | Anteil |
|---|---:|---:|
| richtig | 96 | 3,57 % |
| falsch | 18 | 0,67 % |
| mehrdeutig | 1.447 | 53,83 % |
| nicht zugeordnet | 1.127 | 41,93 % |

Die lokale Diagnose zählte 2.653 eindeutig erscheinende Kontexttreffer, 224 lokal mehrdeutige Treffer, 57.117 Empfänge ohne passenden gebundenen Kontext und 60 Treffer auf bereits abgelaufene Kontexte. Der Unterschied zwischen „lokal eindeutig“ und technisch richtig ist entscheidend: Dieselbe Signatur kann zu mehreren Episoden gehören, obwohl ein einzelnes Neuron nur eine Signatur sieht.

## G. Identische und unterschiedliche Handlungen

| Folge | richtig | falsch | mehrdeutig | nicht zugeordnet |
|---|---:|---:|---:|---:|
| AB→AB | 16 | 3 | 242 | 187 |
| AB→BA | 16 | 3 | 242 | 187 |
| AB→AA | 16 | 3 | 242 | 187 |
| AA→AA | 16 | 3 | 242 | 187 |
| BB→BB | 16 | 3 | 237 | 192 |
| alternierend | 16 | 3 | 242 | 187 |

Identische aufeinanderfolgende Handlungen sind nicht zuverlässig unterscheidbar. Das ist kein Auswertungsfehler: Gleiche Grenz- und Motoraktivität enthält keine Information über „erstes“ oder „zweites“ Auftreten. Eine technische Handlungsnummer hätte die Statistik verbessert, wäre aber genau die verbotene Scheinlösung.

Auch unterschiedliche Reize sind nur unterscheidbar, wenn sie tatsächlich unterschiedliche erreichbare motorische oder verteilte Kontextaktivität erzeugen. Die semantischen Texte `AB`, `BA`, `AA` und `BB` werden nicht als vorgegebene Kontextbedeutungen verwendet.

## H. Verzögerte, überlappende und vertauschte Rückmeldung

| Rückmeldung | richtig | falsch | mehrdeutig | nicht zugeordnet |
|---|---:|---:|---:|---:|
| R0 unmittelbar | 18 | 0 | 227 | 139 |
| R1 verzögert | 12 | 0 | 257 | 115 |
| R2 überlappend | 18 | 0 | 227 | 139 |
| R3 vertauscht | 0 | **18** | 235 | 131 |
| R4 fehlend | 0 | 0 | 0 | 384 |
| R5 gleiche Bewertung | 30 | 0 | 244 | 110 |
| R6 unterschiedliche Bewertung | 18 | 0 | 257 | 109 |

Eine Verzögerung von 16 Ereigniszeiteinheiten senkte die Zahl richtiger Zuordnungen von 18 auf 12 und erhöhte Mehrdeutigkeit. Bei Ablauf vor Rückmeldung (K4) waren alle 64 Läufe nicht zugeordnet. Die vertauschte Reihenfolge R3 ist der schärfste Sicherheitsbefund: 18 Rückmeldungen wurden eindeutig der falschen Episode zugeordnet.

## I. Rückmeldungsinduzierte Störung

Kontextspuren und Eligibility tragen getrennte Herkunftskennzeichen. K11 erzeugte rückmeldungsinduzierten Kontext und registrierte 76 entsprechende lokale Treffer. Diese Zustände wurden nicht als vorbestehende Handlungs-Eligibility gezählt.

Die Herkunftstrennung verhindert rückwirkende statistische „Reparatur“, beseitigt die neuronale Störung aber nicht. Rückmeldung bleibt reguläre Aktivität, kann feuern und neue lokale Zustände erzeugen. Deshalb muss jede spätere Lernregel weiterhin nur vorbestehende, regulär entstandene Berechtigungen berücksichtigen.

## J. Kontrollmatrix K0 bis K15

Alle 16 Kontrollen wurden für 64 Seeds tatsächlich ausgeführt:

- K0 ohne Kontext, K1 ohne Bindung, K2 ohne Feedback, K3 ohne vorherige Handlung, K4 nach Ablauf und K15 vollständig deaktiviert blieben durchgehend nicht zugeordnet.
- K5 mit falscher/vertauschter Folge ergab 39 mehrdeutige, 3 falsche und 22 nicht zugeordnete Läufe.
- K6 identische Handlungen und K7 unterschiedliche Handlungen bei gleicher Bewertung ergaben jeweils nur 5 richtige, 41 mehrdeutige und 18 nicht zugeordnete Läufe.
- K8 unterbrach M3 gezielt; Kontext konnte trotzdem entstehen. Dies bestätigt, dass Kontext nicht gleich nachgewiesener motorischer Kausalpfad ist.
- K9 Abschluss ohne nachgewiesenen Pfad ergab 39 mehrdeutige, 3 richtige und 22 nicht zugeordnete Fälle.
- K10 verglich L1 und L2, K14 R1 und R2F. Zusammengefasst ergaben beide Kontrollen 41 richtige und 23 nicht zugeordnete Läufe; die Subvarianten stehen in den Rohdaten.
- K11 wies rückmeldungsinduzierten Kontext getrennt aus.
- K12 ohne C3-Verteilung bildete nur 317 Spuren und 10 Bindungen; alle 64 Läufe blieben nicht zugeordnet.
- K13 verzögerte/störte den Kontext; alle 64 Läufe blieben nicht zugeordnet.

In keiner Kontrollzeile änderte B.3.1 ein synaptisches Gewicht.

## K. Fortlaufender Betrieb, Persistenz und Ressourcen

### Fortlaufender Betrieb

Für 2, 8, 32 und 128 Wiederholungen wurden über die sechs etablierten Seeds 1.020 Phasen ausgeführt. 628 wurden richtig und 392 nicht zugeordnet. Der vereinfachte Einzel-Episodenvergleich in diesem Lauf enthält absichtlich keine konkurrierende technische Episode und misst daher Stabilität, nicht die schwierigere Zwei-Handlungs-Trennschärfe.

Der feste Speicher blieb begrenzt. Es wurden 51.131 Überschreibungen und 49.461 veraltete Ablaufereignisse gezählt; keine davon veränderte Gewichte. Die hohen Zahlen zeigen, dass P2 unter dichter C4-Aktivität häufig rotiert. Die Ereigniswarteschlange blieb deterministisch und begrenzt.

### Persistenz

Alle 64 Format-8-Rundläufe waren vor und nach Fortsetzung fingerprint-identisch. Nach 64 Zeiteinheiten wurden 5.454 gültige und 219 veraltete Kontextabläufe verarbeitet. Aktive Spuren, mehrere lokale Spuren und ausstehende Abläufe wurden korrekt rekonstruiert. Bestehende Regressionen bestätigen weiterhin die Lesbarkeit alter Zustandsvarianten.

### Performance bei 256 Neuronen

| Kontext | Ereignisse pro Episode | maximale Queue | Speicher gesamt | Laufzeit |
|---|---:|---:|---:|---:|
| C0 | 206 | 100 | 106.536 B | 38,17 µs |
| C1/P2 | 216 | 105 | 122.808 B | 43,29 µs |
| C2/P2 | 206 | 100 | 122.408 B | 33,48 µs |
| C3/P2 | 271 | 150 | 126.208 B | 49,05 µs |
| C4/P2 | 271 | 150 | 126.208 B | 43,17 µs |

Die Rohgröße von P2 beträgt 16.384 Byte bei 256 Neuronen. Linear geschätzt sind es 0,64 MB bei 10.000, 6,4 MB bei 100.000 und 64 MB bei 1.000.000 Neuronen. Hinzu kommen Ablaufereignisse und die kleine feste Kontextverschaltung.

## L. Grenzen, Abschlussfragen und Entscheidung

1. **Kann EVE Kontext aus eigener Aktivität bilden?** Ja. C1–C4 erzeugten reproduzierbare lokale Spuren.
2. **Können mehrere Neuronen Kontext teilen?** Ja. C3/C4 verteilten ihn über reale Relais und Ziele.
3. **Kann Eligibility gebunden werden?** Ja. B1 und B2 banden lokal; Mehrdeutigkeit wird erhalten.
4. **Kann Rückmeldung Kontext erreichen?** Ja, aber mit vielen Empfängen ohne passenden Bezug.
5. **Werden unterschiedliche Handlungen zuverlässig unterschieden?** Nein.
6. **Werden identische Folgehandlungen unterschieden?** Nein; die erforderliche Information fehlt.
7. **Wie verlässlich ist verzögerte Zuordnung?** Bei 16 Einheiten nicht schlechter in der Zahl richtiger Fälle, aber überwiegend mehrdeutig oder unzugeordnet.
8. **Wie häufig ist falsche Zuordnung?** 18/2.688 insgesamt; alle 18 in R3, zusätzlich 3/64 in K5.
9. **Was geschieht bei mehreren Kontexten?** B1/B2 markieren Mehrdeutigkeit; die Forschungsdiagnose ordnet nicht künstlich zu.
10. **Was geschieht nach Ablauf?** Die Rückmeldung bleibt unzugeordnet beziehungsweise wird lokal als abgelaufen gezählt.
11. **Wie stark stört Rückmeldung?** K11 registrierte 76 rückmeldungsinduzierte Kontexttreffer; Herkunftstrennung funktioniert.
12. **Welche Ressourcen entstehen?** Fester linearer P2-Speicher, begrenzte Verdrahtung und zusätzliche Ablaufereignisse; bei 256 Neuronen etwa 19,7 KB Gesamtdifferenz C0→C4.
13. **Welche Unterscheidung bleibt unmöglich?** Zeitlich verschiedene, aber neuronal identische Handlungen ohne zusätzliches unterscheidendes Signal.
14. **Reicht die Qualität für B.4?** Nein.

### Entscheidungsvorlage

Der Übergang zu einer rückmeldungsabhängigen Gewichtsänderung wird nicht empfohlen. Vor B.4 sollte eine weitere Forschungsstufe untersuchen, wie zeitliche Reihenfolge oder Episodentrennung neuronal repräsentiert werden kann, ohne eine technische Handlungs-ID einzuführen. Mindestkriterium wäre eine vorab festgelegte, deutlich höhere eindeutige Trefferquote bei zugleich begrenzten Fehlzuordnungen in R2/R3 und bei identischen Handlungen.

Bis dahin bleiben Gewichtsänderungen durch Bewertung gesperrt.

## Reproduzierbarkeit

```sh
make test
scripts/run-research-stage-b31.sh
```

Rohdaten:

- `research-stage-b31-contexts.csv`
- `research-stage-b31-sequences.csv`
- `research-stage-b31-controls.csv`
- `research-stage-b31-continuous.csv`
- `research-stage-b31-persistence.csv`
- `research-stage-b31-performance.csv`
- `research-stage-b31-parameters.csv`
