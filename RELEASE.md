# Release process

This document describes how to prepare a release of Water Temperature Sensor.

## 1. Decide the version and codename

Use Semantic Versioning (`MAJOR.MINOR.PATCH`) and one-word codenames from the
project's chemistry/particle theme (Atom, Ion, …).

Current releases:

| Version | Codename | Release type | Status |
| --- | --- | --- | --- |
| `v0.0.1` | Atom | 🚧 Pre-release | Historical baseline |
| `v1.0.0` | Ion | ✅ Stable release | Current |

The pre-release status is expressed in the release document and GitHub release
settings, while the version remains the plain `v0.0.1` tag.

## 2. Prepare the sketch

1. Confirm the sketch compiles cleanly for Uno **and** Nano
   (`arduino-cli compile --fqbn arduino:avr:uno .` — CI enforces the same).
2. Run the behavioural checklist in [docs/Development.md](docs/Development.md#testing-a-change):
   boot self-test chirps, normal path, alarm path (arm at 30 °C, release below
   28 °C), and fault path (probe disconnect and recovery).
3. Update the `Version:` line in the sketch header comment.
4. If the release supersedes the current sketch in a way worth preserving,
   copy the outgoing version into its own sketch folder under `archive/` as
   `archive/watertempraturesensor_v<x>_<y>_<z>_<codename>/<same name>.ino`
   with a historical-artifact header. Each archived sketch keeps its own
   folder (name matching the `.ino`) so it never merges into the current
   sketch and CI can still compile-verify it.

## 3. Update version references

Update:

- `README.md` version badge;
- `CHANGELOG.md`;
- `docs/releases/v<Version>.md`;
- `SECURITY.md` supported-versions table; and
- `CHECKSUMS.sha256`.

## 4. Write the changelog entry

Use only relevant Keep a Changelog categories:

```markdown
## [1.1.0] — "Codename" — YYYY-MM-DD

### Added
### Changed
### Improved
### Fixed
### Removed
### Deprecated
### Security
### Documentation
```

The changelog is a concise history. Do not copy the full release narrative into
it.

## 5. Write the release document

Create `docs/releases/v<Version>.md` following the established template
(see [v1.0.0 "Ion"](docs/releases/v1.0.0.md)): Overview, Objectives,
Added/Changed sections, Compatibility, Contributors, and Release Summary.

## 6. Refresh checksums

```bash
sha256sum WaterTempratureSensor.ino archive/*.ino > CHECKSUMS.sha256
```

Verify with `sha256sum -c CHECKSUMS.sha256`.

## 7. Tag and publish

```bash
git tag -a v<Version> -m "v<Version> — <Codename>"
git push origin v<Version>
gh release create v<Version> --title "v<Version> — <Codename>" \
  --notes-file docs/releases/v<Version>.md
```

Mark pre-releases with `--prerelease`.
