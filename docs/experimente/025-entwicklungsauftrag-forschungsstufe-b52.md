# Entwicklungsauftrag 025 – Forschungsstufe B.5.2

## Lokale Gedächtnisstabilität und Trennung synaptischer von neuronaler Interferenz

**Projekt:** EVE – Emergent Virtual Evolution
**Technologie:** C++17
**Ausgangsbasis:** Konsolidierter Forschungsstand B.5.1 auf `main`
**Forschungsbranch:** `codex/research-stage-b52-memory-stability`
**Forschungsstatus:** Kausale Grundlagenforschung ohne vorab freigegebenen neuen Lernmechanismus

---

## 1. Ausgangslage

Die Forschungsstufen B.4.2 bis B.5.1 haben folgende Fähigkeiten und Eigenschaften experimentell untersucht:

- Ereignisgesteuerte neuronale Verarbeitung.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokales G3-Ereignisgedächtnis.
- Zeitlich begrenzte synaptische Eligibility.
- Drei-Faktor-Plastizität.
- Angeborene geschützte Exploration.
- Lokale inhibitorische Konkurrenz.
- Persistente neuronale Zustände.
- Erfahrungsabhängigen Erwerb und Wiedererwerb autonomer Handlungen.

B.5.1 zeigte, dass die vorhandenen Mechanismen grundsätzlich ausreichen, um Reiz-Handlungs-Beziehungen unter natürlichen Erfahrungen erstmalig zu erwerben.

Allerdings bleiben die erworbenen Fähigkeiten instabil.

### Zentrale Ergebnisse aus B.5.1

Untersucht wurden:

- 768 neuronale Netze.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnung.
- 512 Trainingsepisoden je Netz.
- Insgesamt 393.216 natürliche Erfahrungen.
- 1.536 sensorische Lernpfade.

Ergebnisse:

- 1.536/1.536 Pfade wurden mindestens einmal autonom erfolgreich.
- Alle 1.536 Pfade verloren später mindestens einmal ihre autonome Abrufbarkeit.
- Alle 1.536 Pfade wurden mindestens einmal erneut erworben.
- Insgesamt wurden 66.159 Verluste und 65.284 Wiedererwerbe registriert.
- Nach 64 weiteren Erfahrungen waren nur noch 43,88 % beziehungsweise 45,44 % der ursprünglich erworbenen Bindungen korrekt abrufbar.
- Alle 768 Netze erreichten mindestens einmal gleichzeitig korrekte Antworten für beide Reize.
- Am Ende waren nur 140/768 Netze gleichzeitig korrekt.

### Begleiterscheinungen der Verluste

Die unmittelbaren Verlustepisoden wurden unter anderem folgenden Klassen zugeordnet:

| Beobachtung | Verlustereignisse |
|---|---:|
| Inhibitorische Gewichtsänderung als Hauptkategorie | 28.096 |
| Andere sensorische Gewichtsänderung | 22.625 |
| Keine synaptische Gewichtsänderung | 12.452 |
| Negative Abschwächung des Zielpfads | 1.888 |
| Weitere konkurrierende positive Änderungen | 1.098 |

Diese Kategorien wurden anhand der jeweils beobachteten Gewichtsänderungen vergeben.

**Die Klassifikation ist zunächst eine Beschreibung der Ereignisse und kein eindeutiger Kausalnachweis.**

### Kausale Gegenproben

Beim ersten Fähigkeitsverlust konnte eine isolierte Rücknahme inhibitorischer Änderungen nur etwa 17–23 % der Verluste verhindern.

Die Rücknahme sensorischer Änderungen verhinderte etwa 4 %.

Neutrale Modulation beziehungsweise deaktivierte Plastizität verhinderte etwa 18–24 %.

Die Verluste lassen sich damit nicht auf einen einzigen synaptischen Mechanismus reduzieren.

Besonders wichtig ist die Beobachtung, dass 12.452 Verlustereignisse ohne synaptische Gewichtsänderung auftraten.

Das könnte auf transiente neuronale Zustände hinweisen.

Ob tatsächlich eine Abrufstörung vorliegt, muss jedoch erst experimentell nachgewiesen werden.

---

## 2. Zentrale Forschungsfrage

**Welche lokalen neuronalen und synaptischen Zustände bestimmen, ob eine bereits erlernte Reiz-Handlungs-Beziehung autonom abrufbar bleibt?**

Dazu müssen drei Ursachenbereiche getrennt untersucht werden:

1. Veränderung erregender sensorischer Verbindungen.
2. Veränderung inhibitorischer Verbindungen und ihrer lokalen Lernberechtigung.
3. Veränderung transienter neuronaler Zustände bei unveränderten synaptischen Gewichten.

Das Ziel ist ausdrücklich nicht, erworbene Gewichte möglichst lange unverändert zu halten.

Das Ziel ist, die tatsächlichen Bedingungen für zuverlässige Verfügbarkeit und sinnvolle Veränderbarkeit erlernter Fähigkeiten zu verstehen.

---

## 3. Forschungshypothesen

### H1 – Transiente Abrufstörung

Eine synaptisch gespeicherte Beziehung bleibt vorhanden, wird aber aufgrund momentaner Membranpotenziale, Erholungszustände oder lokaler Restaktivität vorübergehend nicht korrekt abgerufen.

### H2 – Nichtursächliche inhibitorische Kreditzuordnung

Positive oder negative Modulation verändert inhibitorische Synapsen, deren Beteiligung an der bewerteten Handlung nicht kausal nachgewiesen werden kann.

Solche Änderungen können den späteren Abruf anderer erlernter Beziehungen beeinträchtigen.

### H3 – Geteilte sensorische Lernkandidaten

Mehrere Reiz-Handlungs-Beziehungen nutzen teilweise gemeinsame neuronale Verarbeitungswege.

Eine lokal gültige Gewichtsänderung für die aktuelle Erfahrung kann eine bereits vorhandene andere Beziehung beeinflussen.

### H4 – Kombinierte Interferenz

Fähigkeitsverluste entstehen durch das Zusammenwirken mehrerer synaptischer und transienter Änderungen.

Eine isolierte Rücknahme nur eines Faktors ist dann möglicherweise wirkungslos, obwohl dieser Faktor kausal beteiligt ist.

### H5 – Instabile Konkurrenz statt fehlender Speicherung

Die relevanten sensorischen Gewichte sind weiterhin ausreichend stark, ihre Wirkung wird jedoch durch konkurrierende Erregung oder Hemmung überlagert.

### H6 – Messabhängige Zustandsunterschiede

Ein Teil der beobachteten Schwankungen könnte von der zeitlichen und neuronalen Zustandslage während der diagnostischen Reizprüfung abhängen.

Die Unterscheidung zwischen einer einzelnen momentanen Antwort und einer stabilen Fähigkeit muss deshalb methodisch abgesichert werden.

Alle Hypothesen sind getrennt zu prüfen.

Mehrere Ursachen dürfen gleichzeitig zutreffen.

---

## 4. Phase A – Referenz und technische Absicherung

### A1 – Konsolidierte Ausgangsbasis

Der Forschungsbranch B.5.2 muss vom erfolgreich integrierten B.5.1-Forschungsstand ausgehen.

Vor Beginn prüfen:

- Ist B.5.1 tatsächlich in `main` enthalten?
- Sind alle bisherigen Forschungsberichte und Daten vorhanden?
- Sind die zusätzlichen Konzeptdokumente aus `main` erhalten?
- Ist die bisherige Testsuite erfolgreich?

Falls die Integration noch nicht abgeschlossen ist, darf kein vermeintlich konsolidierter Stand angenommen werden.

Der fehlende Integrationsstand ist zu dokumentieren und die vorgesehenen Grundlagen sind zunächst herzustellen.

### A2 – Regression

Reproduziere die relevanten Ergebnisse aus B.5.1:

- Natürlicher Erwerb und Wiedererwerb.
- Verlustereignisse.
- Sensorische Gewichtstrajektorien.
- Inhibitorische Gewichtstrajektorien.
- E1-/E2-Checkpoints.
- Kontrollierte Fortsetzungen.
- Unabhängige Bestätigung.
- Persistenz.

Die bisherigen 509 Kerntests müssen erhalten bleiben.

### A3 – Unveränderte neuronale Architektur

Für alle Referenzversuche gelten:

- 32 Neuronen im bestehenden Forschungsnetz.
- Feuergrenze 2.
- Gewichtsgrenze 3.
- Bestehende G3-Verarbeitung.
- Bestehende `active_phase`-Eligibility.
- Ternäre Modulation.
- Geschützte Exploration.
- Lernbare inhibitorische Verbindungen.

Es werden zunächst keine funktionalen Änderungen vorgenommen.

---

## 5. Phase B – Synaptische Speicherung und momentane Abrufbarkeit trennen

Dieser Abschnitt ist der erste Schwerpunkt von B.5.2.

### B1 – Identische Synapsen, unterschiedliche neuronale Zustände

Identifiziere reale Trainingszustände, in denen:

- Ein Reiz zunächst korrekt autonom beantwortet wird.
- Später die autonome Antwort ausbleibt oder falsch wird.
- Zwischen den untersuchten Zuständen keine synaptische Gewichtsänderung stattgefunden hat.

Erzeuge vollständige Zustandskopien unmittelbar vor und nach dem beobachteten Verlust.

Prüfe ausdrücklich, ob sämtliche Synapsengewichte und strukturellen Verbindungen identisch sind.

Vergleiche zusätzlich:

- Membranpotenziale.
- Erholungs- beziehungsweise Refraktärzustände.
- Lokale Ereignisspeicher.
- Eligibility-Spuren.
- Ereigniswarteschlange.
- Simulierte Zeit.
- Sonstige verhaltenswirksame Runtime-Zustände.

### B2 – Vergleich bei identischen Bedingungen

Ein bloßer Vergleich zweier unterschiedlicher Zeitpunkte ist nicht ausreichend.

Erforderlich sind kontrollierte Zustandskopien, bei denen die zu vergleichenden Faktoren gezielt isoliert werden.

Untersuche mindestens:

- Identische Gewichte mit ursprünglichem transientem Zustand.
- Identische Gewichte mit späterem transientem Zustand.
- Identische Gewichte nach ereignisfreier Zeitfortschreibung.
- Identischer vollständiger Zustand mit wiederholter diagnostischer Reizprüfung.

Eine bloße Zeitfortschreibung darf keine künstliche Plastizität erzeugen.

### B3 – Kontrollierte Wiederherstellung transienter Zustände

Untersuche in diagnostischen Kopien, ob die Wiederherstellung eines früheren transienten Zustands die zuvor erfolgreiche autonome Antwort erneut ermöglicht.

Die folgenden Faktoren sind zunächst einzeln und anschließend kombiniert zu prüfen:

1. Membranpotenziale.
2. Erholungs- beziehungsweise Refraktärzustände.
3. Lokale temporäre Ereignisspuren.
4. Ausstehende neuronale Ereignisse.
5. Weitere tatsächlich relevante Runtime-Zustände.

Die Wiederherstellung muss technisch gültige Zustände erhalten.

Es dürfen keine künstlichen neuronalen Feuerereignisse als vermeintlich erfolgreiche Erinnerung eingefügt werden.

Falls einzelne Zustände über die bestehende öffentliche Diagnoseschnittstelle nicht isoliert verändert werden können, ist dies zu dokumentieren.

Eine gegebenenfalls benötigte experimentelle Diagnoseschnittstelle darf nicht das normale Lernverhalten verändern.

### B4 – Trennung von Speicherung und Abruf

Unterscheide explizit:

**Synaptische Speicherung:** Die relevanten erlernten Gewichte und Verbindungen bleiben vorhanden.

**Funktionaler Abruf:** Der aktuelle neuronale Gesamtzustand erzeugt die richtige eindeutige Antwort.

**Stabile Abrufbarkeit:** Die richtige Antwort bleibt unter vorab definierten natürlichen Zustandsbedingungen reproduzierbar.

Ein vorübergehender Abruffehler darf nicht automatisch als Löschung der gespeicherten Verbindung klassifiziert werden.

Umgekehrt darf das bloße Vorhandensein starker Gewichte nicht als Nachweis einer erhaltenen nutzbaren Fähigkeit gelten.

---

## 6. Phase C – Inhibitorische Eligibility kausal untersuchen

### C1 – Ereignisbezogene Kandidatenanalyse

Erfasse für jede inhibitorische Gewichtsänderung:

- Senderneuron.
- Empfängerneuron.
- Synapsen-ID.
- Gewicht vor und nach der Modulation.
- Inhibitorische Wirkung.
- Zugehörigen lokalen Eligibility-Eintrag.
- Eligibility-Generation.
- Beteiligte Eingangsimpulse.
- Zeitpunkt der Aktivität.
- Membranpotenzial vor und nach Integration.
- Resultierende neuronale Feuerereignisse.
- Tatsächlich bewertete motorische Handlung.

Unterscheide zwischen:

- Zeitlich gültiger Eligibility.
- Funktionaler neuronaler Beteiligung.
- Kausal nachweisbarem Beitrag zur bewerteten Handlung.

Diese drei Eigenschaften dürfen nicht gleichgesetzt werden.

### C2 – Nichtursächliche inhibitorische Kandidaten

Identifiziere Fälle, in denen eine inhibitorische Synapse verändert wird, obwohl ihre bisherige Aktivität nicht notwendig für die bewertete Handlung war.

Dazu sind diagnostische Ablationen vor der Modulation erforderlich.

Untersuche, ob das Unterdrücken beziehungsweise Wiederherstellen ihres Beitrags die ursprüngliche vollständige Motorentscheidung verändert.

Eine wirkungslose Einzelablation beweist bei redundanten oder konkurrierenden Eingängen noch keine generelle Irrelevanz.

Wo erforderlich, sind kombinierte Eingriffe vorzusehen.

### C3 – Spätere Wirkung

Verfolge dieselbe inhibitorische Gewichtsänderung über weitere natürliche Erfahrungen.

Untersuche:

- Wird eine zuvor korrekte Beziehung beeinträchtigt?
- Entsteht eine fehlende Antwort?
- Entsteht eine Doppelantwort?
- Wird eine andere Beziehung begünstigt?
- Wird die Änderung später wieder aufgehoben?

### C4 – Diagnostische Gegenproben

Vergleiche aus identischen Ausgangszuständen:

- Unveränderte inhibitorische Plastizität.
- Neutralisierte einzelne inhibitorische Änderung.
- Isoliert übernommene inhibitorische Änderung.
- Rücknahme ausgewählter nichtursächlich wirkender Änderungen.
- Unveränderte sensorische Plastizität bei diagnostisch unterdrückter inhibitorischer Gewichtsänderung.
- Gleiche Aktivität ohne weitere Plastizität.

Diese Eingriffe sind ausschließlich diagnostisch zulässig.

Die äußere Klassifikation einer Synapse als „nichtursächlich“ darf nicht direkt zu einer neuen Produktionslernregel werden.

---

## 7. Phase D – Gemeinsame sensorische Lernkandidaten

### D1 – Überlappung neuronaler Aktivität

Untersuche, welche sensorischen und nachfolgenden neuronalen Verbindungen während Erfahrungen mit X beziehungsweise Y aktiv werden.

Erfasse:

- Gemeinsame Empfängerneuronen.
- Gemeinsame Eligibility-Spuren.
- Gemeinsam aktive synaptische Kandidaten.
- Konkurrenz zwischen sensorischen Gewichten.
- Auswirkungen auf inhibitorische Verbindungen.

### D2 – Lokal berechtigte, aber störende Änderungen

Identifiziere Fälle, in denen die Modulation einer aktuell erfolgreichen oder erfolglosen Handlung eine Verbindung verändert, die für eine andere erlernte Beziehung funktional wichtig ist.

### D3 – Kausale Zustandskopien

Vergleiche:

1. Vollständige natürliche Gewichtsänderung.
2. Keine Gewichtsänderung.
3. Ausschließlich die sensorischen Änderungen des aktuellen Reizes.
4. Ausschließlich Änderungen anderer sensorischer Wege.
5. Ausschließlich inhibitorische Änderungen.
6. Kontrollierte Kombinationen dieser Eingriffe.

Prüfe jeweils beide Beziehungen.

Die Varianten 3 bis 6 sind Diagnoseinstrumente, keine zulässigen neuronalen Lernregeln.

### D4 – Erhalt ohne Starrheit

Untersuche, ob eine zuvor erworbene Beziehung erhalten bleibt, obwohl ihre Gewichte weiterhin grundsätzlich plastisch sind.

Ein vollständig geschütztes Gewicht gilt nicht automatisch als bessere Gedächtnislösung.

Eine erlernte Beziehung muss bei veränderten Umweltbedingungen weiterhin verändert werden können.

---

## 8. Phase E – Kombinierte Kausalität

B.5.1 zeigte, dass einzelne Rollbacks nur einen begrenzten Teil der Verluste verhindern.

B.5.2 muss deshalb kombinierte Ursachen untersuchen.

### E1 – Faktoren

Mindestens folgende Bereiche sind getrennt zu betrachten:

- S: Sensorische Gewichtsänderungen.
- I: Inhibitorische Gewichtsänderungen.
- T: Transiente neuronale Zustandsänderungen.

### E2 – Faktorielle Zustandskopien

Für geeignete reale Verlustereignisse sind kontrollierte Kombinationen dieser drei Faktoren zu untersuchen.

Mindestens:

| Variante | S | I | T |
|---|---|---|---|
| V0 | alter Zustand | alter Zustand | alter Zustand |
| V1 | neuer Zustand | alter Zustand | alter Zustand |
| V2 | alter Zustand | neuer Zustand | alter Zustand |
| V3 | alter Zustand | alter Zustand | neuer Zustand |
| V4 | neuer Zustand | neuer Zustand | alter Zustand |
| V5 | neuer Zustand | alter Zustand | neuer Zustand |
| V6 | alter Zustand | neuer Zustand | neuer Zustand |
| V7 | neuer Zustand | neuer Zustand | neuer Zustand |

Dabei muss sichergestellt sein, dass die kombinierten Zustände technisch gültig sind.

Falls einzelne Kombinationen aufgrund zwingender neuronaler Abhängigkeiten unzulässig sind, dürfen sie nicht künstlich als reguläre Zustände dargestellt werden.

### E3 – Wechselwirkungen

Untersuche:

- Kann sensorische Veränderung allein einen Verlust erzeugen?
- Kann inhibitorische Veränderung allein einen Verlust erzeugen?
- Kann ein transienter Zustand allein einen Verlust erzeugen?
- Entsteht ein Verlust erst durch Kombination zweier Faktoren?
- Können sich Faktoren gegenseitig kompensieren?

### E4 – Zuordnung kausaler Beiträge

Ein Faktor gilt nicht schon deshalb als irrelevant, weil seine isolierte Rücknahme den Verlust nicht verhindert.

Bei kombinierten Ursachen müssen auch Interaktionen berücksichtigt werden.

Notwendige, hinreichende und lediglich beitragende Faktoren sind begrifflich getrennt auszuweisen.

---

## 9. Phase F – Stabilität zweier gleichzeitig gelernter Beziehungen

### F1 – Ausgangszustände

Identifiziere reale Netzzustände, in denen beide Reize autonom eindeutig korrekt beantwortet werden.

Untersuche getrennt:

- Ersten gleichzeitig erfolgreichen Zustand.
- Spätere erneut gleichzeitig erfolgreiche Zustände.
- Zustände nach längerer stabiler Phase.

### F2 – Weitere natürliche Erfahrungen

Verfolge aus diesen Zuständen die nächsten:

- 1 Erfahrung.
- 4 Erfahrungen.
- 8 Erfahrungen.
- 16 Erfahrungen.
- 32 Erfahrungen.
- 64 Erfahrungen.

Erfasse jeweils:

- Beide Beziehungen korrekt.
- Nur X korrekt.
- Nur Y korrekt.
- Beide falsch oder nicht abrufbar.
- Gewichtsänderungen.
- Transiente Zustandsänderungen.

### F3 – Fehlerursache je Beziehung

Wenn eine von zwei Fähigkeiten ausfällt, muss die auslösende Episode einzeln untersucht werden.

Insbesondere:

- Welcher Reiz wurde tatsächlich präsentiert?
- Welche Handlung wurde ausgeführt?
- Welche Modulation erfolgte?
- Welche Synapsen wurden verändert?
- Welche transienten Zustände wurden verändert?
- Welche Kombination erklärt die veränderte autonome Ausgabe?

### F4 – Gemeinsame Stabilität

Ein diagnostischer Eingriff darf nicht ausschließlich anhand der Verbesserung einer Beziehung bewertet werden.

Er muss gleichzeitig prüfen, ob die zweite Beziehung erhalten bleibt.

---

## 10. Phase G – Kontrollierte transiente Zustandsprüfungen

Dieser Abschnitt soll methodische Messartefakte ausschließen.

### G1 – Probewirkung

Untersuche, ob die diagnostische Reizprüfung selbst transiente neuronale Zustände verändert.

Vergleiche:

- Einen einzelnen Probeimpuls.
- Wiederholte Probeimpulse aus jeweils identischen Kopien.
- Wiederholte Probeimpulse auf derselben fortgesetzten Runtime.
- Unterschiedliche ereignisfreie Abstände zwischen Prüfungen.

Dabei darf keine weitere synaptische Plastizität stattfinden.

### G2 – Zeitabhängigkeit

Prüfe die autonome Antwort bei unterschiedlichen, vorab festgelegten Zeitabständen nach einer Lernerfahrung.

Die Abstände sind als simulierte neuronale Zeit zu betrachten, nicht als globale Lern-Ticks.

### G3 – Zustandsnormalisierung

Eine diagnostische Normalisierung transienter Zustände darf untersucht werden, sofern:

- Sie technisch eindeutig definiert ist.
- Sie keine Gewichte verändert.
- Sie keine richtigen Antworten erzwingt.
- Sie alle betroffenen neuronalen Rollen gleichartig behandelt.
- Sie ausschließlich zur Ursachenanalyse verwendet wird.

Vergleiche normalisierten Abruf und natürlichen Abruf getrennt.

Eine Verbesserung nach Normalisierung ist noch kein Nachweis für einen geeigneten Produktionsmechanismus.

### G4 – Persistenz

Prüfe, ob gespeicherte und wiederhergestellte Runtime-Zustände exakt dieselben transienten Eigenschaften und autonomen Ergebnisse aufweisen.

Die Zustandswiederherstellung darf nicht unbemerkt eine neuronale Normalisierung durchführen.

---

## 11. Phase H – Bedingte lokale Forschungsvariante

**Es ist zunächst kein neuer Lern- oder Gedächtnismechanismus freigegeben.**

Eine minimale Forschungsvariante ist erst zulässig, wenn eine der vorherigen Untersuchungen einen konkreten lokalen Fehler kausal belegt.

### Mögliche, ausdrücklich offene Forschungsrichtungen

- Präzisere lokale inhibitorische Kreditzuordnung.
- Veränderung der Beteiligung gemeinsam aktiver synaptischer Kandidaten.
- Lokale Behandlung transienter Erregungs- und Erholungszustände.
- Reduzierung bestimmter nachweislich störender Wechselwirkungen.

Diese Möglichkeiten sind keine Implementierungsanweisung.

### Anforderungen

Jede neue Variante muss:

1. Einen isolierten neuronalen Engpass adressieren.
2. Ausschließlich lokal verfügbare Informationen nutzen.
3. Keine richtige Reiz-Handlungs-Zuordnung kennen.
4. Keine globale motorische Entscheidung einführen.
5. Keine gelernten Gewichte pauschal schützen.
6. Keine vorhandenen Erfahrungen automatisch löschen.
7. Ohne globale neuronale Taktung auskommen.
8. Vollständig persistierbar sein.
9. Abschaltbar und kausal gegen die Referenz prüfbar sein.
10. Die Anpassungsfähigkeit an veränderte Umweltbedingungen erhalten.

Die Einführung einer neuen Zustandsvariable allein ist kein Forschungsfortschritt.

### Freigabegrenze

Falls die Ursachenanalyse keinen einzelnen geeigneten Eingriff rechtfertigt, bleibt die Forschungsstufe rein diagnostisch.

Ein negativer Befund ist ausdrücklich zulässig.

---

## 12. Kontrollgruppen

Mindestens folgende Gruppen sind vorzusehen:

| Gruppe | Beschreibung |
|---|---|
| K0 | Unveränderte B.5.1-Referenz |
| K1 | No-Learning |
| K2 | Feste inhibitorische Verbindungen |
| K3 | Lernbare inhibitorische Verbindungen |
| K4 | Keine weitere synaptische Plastizität |
| K5 | Diagnostische sensorische Rücknahme |
| K6 | Diagnostische inhibitorische Rücknahme |
| K7 | Diagnostische transiente Zustandswiederherstellung |
| K8 | Kombinationen aus S, I und T |
| K9 | Bedingte minimale lokale Forschungsvariante |
| K10 | Identische Variante mit deaktivierter neuer Funktion |

K9 und K10 dürfen nur bei ausreichender kausaler Begründung implementiert werden.

Alle diagnostischen Eingriffe müssen als solche gekennzeichnet sein.

Sie dürfen nicht stillschweigend Bestandteil der allgemeinen Lernarchitektur werden.

---

## 13. Lernaufgaben

Alle zentralen Varianten durchlaufen:

1. Einzelreiz X→A.
2. Einzelreiz Y→B.
3. Sequenziell X→A, danach Y→B.
4. Sequenziell Y→B, danach X→A.
5. Streng alternierende Erfahrungen.
6. Zufällig gemischte Erfahrungen.
7. Inverse Zuordnung X→B und Y→A.
8. Umweltumkehr ohne neuronalen Reset.
9. Retention ohne weitere Plastizität.

Zusätzlich verpflichtend:

- Abrufverlust ohne Gewichtsänderung.
- Abrufverlust durch inhibitorische Änderung.
- Abrufverlust durch sensorische Änderung.
- Kombinierter Abrufverlust.
- Wiederherstellung einer verlorenen Antwort.
- Gleichzeitiger Erhalt beider Reizbeziehungen.
- Veränderbarkeit einer zuvor stabilen Beziehung nach Umweltumkehr.

---

## 14. Primäre Messgrößen

### E1 – Autonomer Erfolg

E1 wird aus einer vollständigen Zustandskopie ohne zusätzliche Exploration und ohne weitere Plastizität ermittelt.

Es zählt ausschließlich eine eindeutige richtige motorische Ausgabe.

### E2 – Explorationsgestützter Erfolg

E2 wird weiterhin getrennt aus einer identischen Ausgangskopie ermittelt.

### Zusätzliche B.5.2-Metriken

Erforderlich sind:

- Anzahl abrufbarer Bindungen bei unveränderten Gewichten.
- Anzahl transient bedingter Ausfälle.
- Anzahl sensorisch bedingter Ausfälle.
- Anzahl inhibitorisch bedingter Ausfälle.
- Anzahl kombinierter Ausfälle.
- Anteil kausal verhinderbarer Verluste.
- Anteil wiederherstellbarer autonomer Antworten.
- Dauer gleichzeitig korrekter Beziehungen.
- Einfluss diagnostischer Eingriffe auf die jeweils andere Beziehung.
- Anzahl der Zustände mit unveränderter Synapsenstruktur, aber verändertem Abruf.
- Unterscheidung zwischen einmaligem Probeausfall und reproduzierbarem Fähigkeitsverlust.

Ein wiederhergestellter Abruf durch diagnostische Manipulation ist nicht als natürlich erlernte neue Fähigkeit zu zählen.

---

## 15. Statistische Versuchsgestaltung

### Entdeckung

Mindestens:

- 64 gepaarte Seeds.
- Drei unabhängige Zufallsstromfamilien.
- Normale und inverse Zielzuordnung.
- Identische äußere Aufgabenbedingungen.
- Dokumentierte unabhängige Zufallsquellen.

### Bestätigung

Mindestens 64 zusätzliche, bislang unbenutzte Seeds je zentraler Stromfamilie und Zielzuordnung.

Bestätigungsdaten dürfen nicht zur Auswahl einer neuen Regel verwendet werden.

### Trainingsdauer

- Bestehende 128-Episoden-Referenz.
- Natürliche 512-Episoden-Verläufe.
- Längere Versuche nur bei konkreter Notwendigkeit und vorab festgelegtem Untersuchungsziel.

### Statistische Auswertung

Erforderlich sind:

- Seedbezogene Erfolgsquoten.
- Gepaarte Zustandskopie-Vergleiche.
- 95-%-Bootstrap-Konfidenzintervalle.
- Clusterbewusste Auswertung wiederholter Ereignisse.
- Verwechslungsmatrizen.
- Ablationsmatrizen.
- Synaptische und transiente Zustandstrajektorien.
- Getrennte Untersuchung erster und späterer Verluste.
- Reproduktion der Kausalbefunde in unabhängigen Daten.

Die Auswahl diagnostischer Verlustereignisse muss vorab definiert werden.

Nicht beobachtete Ereignisse, technische Abbrüche und ungültige Zustände sind ausdrücklich auszuweisen.

---

## 16. Allgemeine Erfolgskriterien

Für eine neu vorgeschlagene neuronale Funktion bleiben die bisherigen übergeordneten Anforderungen bestehen:

- Mindestens 75 % autonome E1-Leistung für X.
- Mindestens 75 % autonome E1-Leistung für Y.
- Mindestens 48/64 Seeds erfüllen beide Schwellen gleichzeitig.
- Mindestens 15 Prozentpunkte gepaarter Vorteil gegenüber No-Learning.
- Vollständig positives 95-%-Konfidenzintervall.
- Unabhängige Reproduktion.
- Erfolg über drei unabhängige Erfahrungsstromfamilien.
- Normale und inverse Rollen.
- Retention ohne weitere Plastizität.
- Erhaltene Fähigkeit zur Umweltanpassung.

### Spezifischer Forschungserfolg B.5.2

B.5.2 kann auch ohne Erreichen dieser allgemeinen Ziele wissenschaftlich erfolgreich abgeschlossen werden.

Dafür ist erforderlich, dass:

1. Synaptische Speicherung und momentane Abrufbarkeit experimentell getrennt wurden.
2. Der Einfluss transienter neuronaler Zustände kausal eingegrenzt ist.
3. Die Rolle inhibitorischer Eligibility überprüft wurde.
4. Gemeinsame sensorische Lernkandidaten untersucht wurden.
5. Kombinierte Ursachen über kontrollierte Zustandskopien bewertet wurden.
6. Der Einfluss auf beide gleichzeitig erlernten Beziehungen dokumentiert ist.
7. Offene Ursachen ausdrücklich benannt werden.

---

## 17. Architekturgrenzen

Unverändert gelten:

- C++17.
- Vollständig ereignisgesteuerte Verarbeitung.
- Kein globaler neuronaler Tick.
- Binäre Feuerereignisse.
- Gewichtete erregende und hemmende Synapsen.
- Lokale G3-Verarbeitung.
- Zeitlich begrenzte Eligibility.
- Drei-Faktor-Plastizität.
- Geschützte angeborene Exploration.
- Keine globalen Gewinnerentscheidungen.
- Keine externen Reizsemantiken im neuronalen Lernzustand.
- Keine technisch vorgegebene richtige Handlung.
- Keine automatische Rücksetzung gelernter Verbindungen.
- Keine permanenten Zeitstempel pro Synapse.
- Keine globale neuronale Zustandsabfrage zur Lernsteuerung.
- Vollständige Persistenz.
- Reproduzierbare Verarbeitung.

Synapsenwachstum bleibt in den primären Experimenten deaktiviert.

Neue Diagnoseschnittstellen sind nur zulässig, wenn sie vom eigentlichen neuronalen Verhalten getrennt bleiben und keine verdeckten Lernmechanismen einführen.

---

## 18. Technische Validierung

Alle bisherigen 509 Kerntests müssen weiterhin bestehen.

Neue Tests sind erforderlich für:

- Identische Gewichte bei unterschiedlichen transienten Zuständen.
- Identische transiente Zustände bei unterschiedlichen Gewichten.
- Reproduzierbaren autonomen Abruf aus identischen Kopien.
- Diagnostische Zeitfortschreibung.
- Wiederholte Reizprüfung ohne Plastizität.
- Vollständige transiente Zustandswiederherstellung.
- Inhibitorische Eligibility.
- Isolierte inhibitorische Gewichtsänderung.
- Kombinierte sensorische und inhibitorische Änderungen.
- Gleichzeitigen Erhalt zweier Beziehungen.
- Kausale Faktorenkombinationen.
- Gültigkeit aller diagnostisch erzeugten Zustände.
- Persistenz neuer Diagnosezustände.
- Abschaltkontrollen neuer Funktionen, sofern implementiert.
- Unveränderte B.4.2- bis B.5.1-Regressionen.

Es dürfen keine unkontrollierten Ereignisschleifen oder unbeschränkt wachsenden Zustände entstehen.

---

## 19. Lieferumfang

Nova-EVE erstellt:

1. Technische Analyse der transienten neuronalen Zustände.
2. Reproduktion des B.5.1-Forschungsstands.
3. Getrennte Untersuchung synaptischer Speicherung und funktionalen Abrufs.
4. Kontrollierte Experimente bei identischen Gewichten.
5. Diagnostische Wiederherstellung transienter Zustände.
6. Kausale Untersuchung inhibitorischer Eligibility.
7. Analyse gemeinsam aktiver sensorischer Kandidaten.
8. Faktorielle Zustandskopie-Ablationen.
9. Untersuchung gleichzeitig erlernter Beziehungen.
10. Zeit- und Evaluationskontrollen.
11. Gegebenenfalls eine minimal begründete lokale Forschungsvariante.
12. Vollständige E1-/E2-Auswertung.
13. Unabhängige Bestätigungsversuche.
14. Ressourcen-, Stabilitäts- und Persistenzprüfung.
15. Vollständige Rohdaten.
16. Regressionstests.
17. Forschungsbericht B.5.2.
18. Entscheidungsvorlage für B.5.3.

Historische Versuchsdaten dürfen nicht überschrieben werden.

---

## 20. Abschlussfragen

Am Ende muss beantwortet werden:

**A.** Wie viele der bisher als Fähigkeitsverlust klassifizierten Ereignisse sind tatsächlich synaptisch bedingt?

**B.** Wie häufig bleibt eine erlernte synaptische Verbindung erhalten, während ihre autonome Abrufbarkeit vorübergehend ausfällt?

**C.** Welche transienten neuronalen Zustände sind für solche Ausfälle notwendig oder hinreichend?

**D.** Welche inhibitorischen Veränderungen tragen kausal zum Verlust einer Fähigkeit bei?

**E.** Werden inhibitorische Synapsen durch Eligibility verstärkt, obwohl sie an der bewerteten Handlung keinen notwendigen Beitrag geleistet haben?

**F.** Welche sensorischen Kandidaten vermitteln Interferenz zwischen zwei gelernten Beziehungen?

**G.** Welche Verluste entstehen erst durch das Zusammenwirken mehrerer Faktoren?

**H.** Kann der korrekte Abruf wiederhergestellt werden, ohne synaptische Gewichte zu verändern?

**I.** Lassen sich zwei Beziehungen gleichzeitig stabil verfügbar halten, ohne ihre spätere Veränderbarkeit technisch zu unterbinden?

**J.** Ist eine neue lokale neuronale Funktion tatsächlich erforderlich, und welcher konkrete Kausalbefund rechtfertigt sie?

Eine neue allgemeine Lernfunktion darf nur aufgrund reproduzierbarer Kausalbefunde freigegeben werden.

Auch eine ausschließlich diagnostische Forschungsstufe ist zulässig.

---

## 21. Forschungsleitlinie

**Eine nicht abrufbare Erinnerung ist nicht zwangsläufig eine verlorene Erinnerung.**

Eine neuronale Verbindung kann weiterhin vorhanden sein, obwohl der aktuelle Zustand des Netzes keine eindeutige Ausgabe ermöglicht.

Ebenso kann eine zuvor erfolgreiche Handlung wieder auftreten, ohne dass das System in der Zwischenzeit etwas neu gelernt hat.

Deshalb müssen wir unterscheiden zwischen:

- Gespeicherter synaptischer Veränderung.
- Momentanem neuronalen Zustand.
- Erfolgreichem Abruf.
- Stabiler Verfügbarkeit.
- Tatsächlicher Anpassung durch neue Erfahrung.

EVE soll keine technisch geschützten richtigen Antworten sammeln.

Sie soll durch Erfahrung neuronale Verbindungen entwickeln, nutzen und bei veränderten Bedingungen anpassen können.

**Das Ziel von B.5.2 ist, den Unterschied zwischen verlorenem Lernen und gestörtem Abruf experimentell nachzuweisen – und die Ursachen instabiler neuronaler Konkurrenz so weit einzugrenzen, dass weitere Architekturentscheidungen auf Erkenntnissen statt auf Vermutungen beruhen.**
