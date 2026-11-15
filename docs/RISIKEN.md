# Risikoregister

**Status:** Entwurf  
**Bewertung:** Eintritt und Auswirkung jeweils niedrig, mittel oder hoch

| ID | Risiko | Eintritt | Auswirkung | Erste Gegenmaßnahme |
|---|---|---:|---:|---|
| R-001 | Fest programmierte Lösungswege werden fälschlich als Lernen bewertet. | Hoch | Hoch | Kontrollgruppen und Ablationstests vorab definieren. |
| R-002 | Verdeckte Semantik gelangt über Schnittstellen oder Lehrdaten in den Kern. | Mittel | Hoch | Datenfluss und Verantwortungsgrenzen dokumentieren und prüfen. |
| R-003 | LLM-Lehrer übernimmt faktisch Entscheidungen. | Mittel | Hoch | Lehrer-, Rückmelde- und Entscheidungskanal strikt trennen. |
| R-004 | Ergebnisse sind wegen unvollständiger Zustände nicht reproduzierbar. | Mittel | Hoch | Ausgangszustand, Ereignisse und Zufallszustände versionieren. |
| R-005 | Selbstveränderung zerstört bereits erworbene Fähigkeiten. | Hoch | Hoch | Stabilitätsmetriken, Rücksetzpunkte und begrenzte Versuche nutzen. |
| R-006 | Energieproxy wird mit realem Stromverbrauch verwechselt. | Hoch | Mittel | Beide Messarten getrennt benennen und gemeinsam auswerten. |
| R-007 | Einzelne negative Erfahrung erzeugt dauerhaft dysfunktionale Zustände. | Mittel | Hoch | Homöostase, Gewöhnung und Reversibilität gezielt untersuchen. |
| R-008 | Technische Handlungsfähigkeit wächst schneller als Sicherheitsmodell. | Mittel | Hoch | Externe Schnittstellen standardmäßig begrenzen und einzeln freigeben. |
| R-009 | Anthropomorphe Deutung ersetzt messbare Evidenz. | Hoch | Mittel | operationale Definitionen und beobachtbare Kriterien verwenden. |
| R-010 | Mögliche Hinweise auf subjektives Erleben werden nicht erkannt. | Unklar | Hoch | ethische Prüfpunkte und externe Begutachtung vorsehen. |
| R-011 | Zwölf-Wochen-Ziel fördert unbelegte Abkürzungen. | Mittel | Hoch | Erfolgskriterien nicht zugunsten des Termins abschwächen. |
| R-012 | Fehlende Lizenz oder Datenrechte blockieren Zusammenarbeit. | Mittel | Mittel | vor Gate 0 verbindlich entscheiden. |

## Pflege

Jedes Risiko benötigt vor Beginn der Implementierung eine verantwortliche Person,
ein Frühwarnsignal und eine konkrete Reaktion. Neue Evidenz kann Bewertung und
Gegenmaßnahmen verändern; die Änderung wird im Pull Request begründet.

## Ethische Stop-Kriterien

Ein Versuch wird ausgesetzt und geprüft, wenn mindestens einer dieser Fälle
eintritt:

- unerwarteter Zugriff auf Systeme oder Daten außerhalb des Versuchs,
- nicht mehr kontrollierbare Ausweitung technischer Handlungsmöglichkeiten,
- anhaltende dysfunktionale Zustände ohne klaren Wiederherstellungsweg,
- belastbare Hinweise auf eigene Interessen oder subjektives Erleben,
- fehlende Nachvollziehbarkeit sicherheitsrelevanter Entscheidungen,
- Verletzung vereinbarter Daten-, Ressourcen- oder Interaktionsgrenzen.
