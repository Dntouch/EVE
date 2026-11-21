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
vollständige Ablation ist damit situativ nützlich. K1 zeigt dagegen, dass die
tatsächliche 2→1-Abschwächung in 38/38 Fällen dieselbe falsche Doppelantwort
wie Gewicht 2 erzeugt und situativ neutral ist. Partielle Abschwächung und
vollständige Ablation sind nicht gleichwertig.

Langfristig entstehen sowohl nützliche als auch schädliche, neutrale und
kontextabhängige Unterschiede. Die ausgewählte Synapse kehrt zwischenzeitlich
in 38/38 Fällen zur direkten Funktion zurück, ist aber nach 256 Erfahrungen in
keinem Fall funktional. K2 belegt zwar fähigkeitsrelevante Alternativen in
38/38 Fällen, aber keinen späteren Übergang: Alle 499 verfolgten alternativen
Eingänge existieren vor dem kritischen Verlust; alle 76 kontrolliert
funktionalen und alle 38 ersetzenden Pfade sind bereits vorher funktional.
Der Befund ist vorbestehende verteilte Unterstützung, nicht nachgewiesene
funktionale Reorganisation.

Damit wäre eine allgemeine Erhaltungsregel weder situativ noch langfristig
begründet. Die vorhandene Eligibility ist zeitlich korrekt, unterscheidet den
konkreten Handlungsbeitrag ihrer lokalen Kandidaten aber nicht hinreichend.

## Architekturfolgen

- A – Integrität, B – Kausalität, C – situative Angemessenheit und D –
  Langzeitwirkung bleiben getrennte Ergebnisse.
- W0–W9 und prospektive Registrierung sind reine Forschungsdiagnostik.
- H0–H3, Alternativpfad-Ablationen und die vollständige Funktionszeitleiste
  werden als post-hoc korrigierte Forschungsdiagnostik übernommen.
- K1 und K2 werden als getrennte post-hoc Zusatzproben übernommen; sie
  korrigieren die Bewertung der Teilabschwächung und der zeitlichen Übernahme.
- Der Schutzvertrag schützt angeborene Grenzen, nicht erreichte freie
  Gewichte.
- Growth, interne Exploration und S4-d16 bleiben in Produktion deaktiviert.
- Eine mögliche C.2-Phase untersucht lokale Handlungsbeiträge und verteilte
  Pfade, nicht die Konservierung einzelner erfolgreicher Synapsen.

## Merge-Empfehlung

C.1 wird als Forschungsinfrastruktur, getrennt erhaltene ursprüngliche und
korrigierte Rohdaten sowie dokumentierter Befund nach `main` gemergt.
Experimentelle Interventionen erhalten keine Produktionsfreigabe.
