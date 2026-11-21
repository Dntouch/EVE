# 0038 – Neuronale Reorganisation rechtfertigt keinen Gewichtsschutz

**Status:** angenommen
**Datum:** 21. November 2026
**Ursprung:** Forschungsphase C.1

## Entscheidung

Die C.1-Forschungsinfrastruktur und ihre negativen wie heterogenen Ergebnisse
werden übernommen. Es wird keine neue Produktions-Lernregel und insbesondere
kein pauschaler Schutz, Einfrieren oder Rollback funktional gewordener
Synapsen eingeführt.

## Begründung

Die kritische 2→1-Schreibung ist für den direkten Funktionsverlust notwendig,
greift aber erst nach der bereits ausgeführten Handlung. Eine nachträgliche
Methodenkorrektur mit Vor-Erfahrungs-Ablation zeigt, dass die ausgewählte
Synapse in 38/38 Fällen die falsche Doppelantwort kausal mitverursacht. Die
Abschwächungsrichtung ist damit situativ nützlich; für die unmittelbar nächste
Entscheidung ist die konkrete Schreibung in 0/38 Fällen wirksam.

Langfristig entstehen sowohl nützliche als auch schädliche, neutrale und
kontextabhängige Unterschiede. Die ausgewählte Synapse kehrt zwischenzeitlich
in 38/38 Fällen zur direkten Funktion zurück, ist aber nach 256 Erfahrungen in
keinem Fall funktional. Kontrollierte Einzel- und Kombinationsablationen
belegen in 38/38 Fällen eine fähigkeitsrelevante Übernahme durch alternative
Pfade. Das gilt für den Fähigkeitsabruf, nicht für deren Beteiligung an der
kritischen Fehlererfahrung.

Damit wäre eine allgemeine Erhaltungsregel weder situativ noch langfristig
begründet. Die vorhandene Eligibility ist zeitlich korrekt, unterscheidet den
konkreten Handlungsbeitrag ihrer lokalen Kandidaten aber nicht hinreichend.

## Architekturfolgen

- A – Integrität, B – Kausalität, C – situative Angemessenheit und D –
  Langzeitwirkung bleiben getrennte Ergebnisse.
- W0–W9 und prospektive Registrierung sind reine Forschungsdiagnostik.
- H0–H3, Alternativpfad-Ablationen und die vollständige Funktionszeitleiste
  werden als post-hoc korrigierte Forschungsdiagnostik übernommen.
- Der Schutzvertrag schützt angeborene Grenzen, nicht erreichte freie
  Gewichte.
- Growth, interne Exploration und S4-d16 bleiben in Produktion deaktiviert.
- Eine mögliche C.2-Phase untersucht lokale Handlungsbeiträge und verteilte
  Pfade, nicht die Konservierung einzelner erfolgreicher Synapsen.

## Merge-Empfehlung

C.1 wird als Forschungsinfrastruktur, getrennt erhaltene ursprüngliche und
korrigierte Rohdaten sowie dokumentierter Befund nach `main` gemergt.
Experimentelle Interventionen erhalten keine Produktionsfreigabe.
