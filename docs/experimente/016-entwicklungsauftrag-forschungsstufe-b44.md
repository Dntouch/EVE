# Entwicklungsauftrag 016 – Forschungsstufe B.4.4

## Strukturelles Lernen und synaptische Interferenz

**Projekt:** EVE – Emergent Virtual Evolution
**Ausgangsbasis:** Branch `codex/research-stage-b43-local-experience-separation`
**Forschungsstatus:** Ursachenprüfung, keine Freigabe neuer Lernmechanismen

## Forschungsfrage

Kann EVE unterschiedliche Erfahrungen zuverlässiger lernen und behalten, wenn
das neuronale Netz während des Lernens neue Synapsen bilden darf? Zu trennen
sind unzureichende strukturelle Kapazität, fehlerhafte lokale Zuordnung der
Lernrückmeldung und eine Kombination beider Ursachen.

## Untersuchungsprogramm

Vor jeder Verhaltensänderung werden die 7 erfolgreichen B.4.3-Seeds aus X→Y
und die 6 erfolgreichen Seeds aus Y→X mit den erfolglosen Seeds verglichen.
Gewichte, Kandidaten, zeitliche Überschneidungen, Änderungen, Vergessen,
Verwechslungen, doppelte und fehlende Ausgaben sind zu erfassen. Korrelation
und Ursache müssen ausdrücklich getrennt werden.

Der bestehende Wachstumsmechanismus ist hinsichtlich Auslösung,
Empfängerauswahl, Anfangsgewicht, Grenzen und Wechselwirkung mit G3,
Eligibility und Drei-Faktor-Plastizität zu analysieren. Eine neue Synapse ist
kein Beleg für einen funktionalen Lernbeitrag.

Die gepaarte Matrix umfasst mindestens 64 Seeds je zentraler Gruppe und die
Aufgaben nur X→A, nur Y→B, X→Y, Y→X, alternierend und zufällig gemischt:

- S0: B.4.3-Referenz ohne Wachstum,
- S1: unveränderte Lernregeln mit dynamischem Wachstum,
- S2: zufällige, zielunabhängige Nullgewicht-Zusatzsynapsen vor dem Lernen,
- S3: `active_phase` ohne Wachstum,
- S4: `active_phase` mit dynamischem Wachstum.

Hinzu kommen Kontrollen ohne Plastizität, Auswertung ohne weitere
Plastizität, Persistenz, unbenutzte Bestätigungs-Seeds, B.4.2-/B.4.3-
Regressionen, Ressourcen- und Ereignismessungen. Wachstums-, Aufgaben-,
Explorations- und Feedbackzufall bleiben getrennt.

Zu erfassen sind Synapsenzahl vor und nach dem Lernen, entstandene,
aktivierte und tatsächlich gewichtete neue Verbindungen, Gewichtsverläufe
neuer und bestehender Synapsen, lernwirksame Wege je Reiz, Fremdreizanteile,
Überschreibung, Netzwerkgröße, Speicher und Ereignisverarbeitung. Der Erhalt
der ersten Beziehung nach Erwerb der zweiten und Verhalten ohne weitere
Plastizität sind ausdrücklich zu prüfen.

Verbessert eine Variante das gleichzeitige Lernen, sind ausschließlich in
diagnostischen Zustandskopien später entstandene Synapsen und einzelne
Gewichtsänderungen zurückzunehmen. Zu klären ist, ob getrennte Wege oder nur
eine veränderte Aktivitätsverteilung den Effekt tragen.

## Architekturgrenzen

- C++17 und ereignisgesteuerte Verarbeitung ohne globalen neuronalen Tick,
- binäre Feuerereignisse und gewichtete Eingänge,
- keine semantischen Reizkennungen oder globalen Handlungs-IDs,
- keine technisch erzwungene richtige Handlung,
- keine Löschung vorhandener Erfahrung als vorgegebene Strategie,
- geschützte angeborene Exploration bleibt erhalten,
- keine zielgerichtete Synapsenbildung anhand der extern richtigen Handlung,
- keine neue allgemeine Lernregel ohne experimentellen Notwendigkeitsnachweis.

## Erfolg und Entscheidung

Erforderlich sind gleichzeitig mindestens 75 % eindeutiger Erfolg für X und
Y, mindestens 15 Prozentpunkte gepaarter Vorteil, ein positives
95-%-Konfidenzintervall und Reproduktion in unabhängigen Seeds. Zulässige
Ergebnisse sind: A – Kapazität ausreichend und Zuordnung dominant; B –
zusätzliche Synapsen helfen; C – nur Kombination hilft; D – Ursache bleibt
ungeklärt. Auch ein negatives Ergebnis ist gültig. Eine automatische Freigabe
eines neuen Lernmechanismus erfolgt nicht.

## Lieferumfang

Technische Wachstumsanalyse, Seedvergleich, reproduzierbare S0–S4-Matrix,
Rohdaten und Statistik, Wachstums- und Gewichtsprotokolle, Kausaldiagnosen,
Regressionstests, Forschungsbericht und Entscheidungsvorlage.
