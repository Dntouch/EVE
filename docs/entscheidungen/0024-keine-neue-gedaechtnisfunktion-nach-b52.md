# Entscheidung 0024: Keine neue Gedächtnisfunktion nach B.5.2

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0023

## Kontext

B.5.2 trennt sensorische, inhibitorische und transiente Faktoren in 15.194
realen Verlustzuständen. 14.731 Antworten werden mit dem früheren transienten
Zustand bei unveränderten neuen Gewichten wiederhergestellt; nur 463 bleiben
unter altem transientem Zustand synaptisch verloren. 12.452 Verluste treten
ohne Gewichtsänderung auf.

149.301 inhibitorische Änderungen sind zeitlich berechtigt. 48.867 davon
waren in der bewerteten Episode weder funktional aktiv noch einzeln notwendig.
Ein pauschaler Rollback stabilisiert die beiden Beziehungen dennoch nicht.
Gemeinsame sensorische Empfänger treten in 80.588 Episoden auf, während 296
Detailverluste erst aus Faktorinteraktionen entstehen.

## Entscheidung

Es wird keine neue allgemeine Lern- oder Gedächtnisfunktion eingeführt. K9
und K10 bleiben deaktiviert. Insbesondere werden weder transiente Zustände
automatisch zurückgesetzt noch gelernte Gewichte pauschal geschützt,
inhibitorische Eligibility pauschal verworfen oder diagnostische Rollbacks in
die Produktionsarchitektur übernommen.

Als gesichert gelten:

- synaptische Speicherung und momentaner funktionaler Abruf sind getrennte
  Eigenschaften,
- der vollständige transiente Runtime-Zustand ist in der Detailstichprobe der
  dominante Verlustfaktor,
- diagnostische Proben verändern diesen Zustand selbst,
- sensorische und inhibitorische Änderungen können einzeln kausal sein,
- zeitliche Eligibility beweist weder Aktivität noch kausale Notwendigkeit,
- gemeinsame Kandidaten vermitteln reale Kreuzinterferenz,
- einzelne Rollbacks lösen die gemeinsame Stabilität nicht,
- Persistenz erhält Gewichte, transienten Zustand und Abruf exakt.

## Vorlage für B.5.3

B.5.3 soll den vollständigen transienten Faktor weiter zerlegen. Priorität
haben lokal beobachtbare Eingangs-, Membran-, Ereignisspeicher- und
Synapsenaktivitätszustände sowie die Wirkung eines Probeimpulses. Jede
Diagnose muss aus identischen Kopien erfolgen und beide Beziehungen prüfen.

Eine minimale Forschungsvariante ist erst zulässig, wenn ein einzelner lokal
verfügbarer Zustand oder eine klar begrenzte Interaktion in Entdeckung und
unabhängiger Bestätigung notwendig nachgewiesen wurde. Sie muss Retention und
Umweltumkehr erhalten und darf keine korrekte Handlung kennen.
