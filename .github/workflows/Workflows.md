# Workflows

Workflows use matrices to build Debug and Release configurations for each platform.

Ideally Test workflows would be triggered as a result of a Build workflow, however
there are similar limitations in GitHub Actions that require a more complex setup:

* Status badges require a separate workflow for each badge
* Workflows cannot query the status of the workflow that triggered them
* Workflow dependencies do not track matrices
  * A Test_Release workflow might be invoked after a Build_Debug workflow
  * A Test workflow cannot restart itself if its artifact has not been built yet

To allow distinct status badges for Build vs Test we need separate workflows for each.

As Test workflow matrix cannot reliably test the artifact produced by the prior Build
workflow matrix each Test workflow builds the code again.