.. ##
.. ## Copyright (c) 2016, Lawrence Livermore National Security, LLC.
.. ##
.. ## Produced at the Lawrence Livermore National Laboratory.
.. ##
.. ## All rights reserved.
.. ##
.. ## This file cannot be distributed without permission and
.. ## further review from Lawrence Livermore National Laboratory.
.. ##

****************************************
Axom Development Process Summary
****************************************

This section provides a high-level overview of key Axom software development
topics and includes links to more detailed discussion.


======================================================
Development and Release Cycles
======================================================

The Axom team uses a sprint-based development process. We collect
and track issues (bugs, feature requests, tasks, etc.) and then target a 
set of development tasks (i.e., issues) to complete for each quarterly 
(3 month) release cycles for the project. Within this quarterly time 
framework, developers of individual Axom components may plan and schedule 
work in any way that works for them. However, work performed in each 
3 month release cycle is tracked as a single unified sprint encompassing 
activities for the entire project.

See :ref:`jira-label` for more information about how we do issue tracking
and release planning.


======================================================
Release Version Numbers
======================================================

The Axom team follows the **semantic versioning** scheme for assigning
release numbers. Semantic versioning conveys specific meaning about 
the code and modifications from version to version by the way version
numbers are constructed.

See :ref:`semver-label` for a description of semantic versioning.


======================================================
Branch Development
======================================================

The Axom team follows the **Gitflow** branching model for software development 
and reviews. Gitflow is a common workflow centered around software releases. 
It makes clear which branches correspond to which phases of development and 
those phases are represented explicitly in the structure of the source code 
repository. As in other branching models, developers develop code locally 
and push their work to a central repository.

See :ref:`gitflow-label` for a detailed description of how we use Gitflow.


======================================================
Code Reviews and Acceptance
======================================================

Before any code is merged into one of our main Gitflow branches (i.e., develop 
or master), it must be adequately tested, documented, and reviewed 
for acceptance by other team members. The review process is initiated via 
a *pull request* on the Axom Bitbucket project.

See :ref:`pullrequest-label` for a description of our review process and 
how we use pull requests.


======================================================
Testing and Code Health
======================================================

Comprehensive software testing processes and use of code health tools (e.g., 
static analysis, memory checkers, code coverage) are essential ingredients 
in the Axom development process.

See :ref:`testing-label` for a description of our software testing process,
including *continuous integration*.


======================================================
Software Development Tools
======================================================

In addition to tools listed above, we use a variety of other tools to help
manage and automate our software development processes. The *tool philosophy*
adopted by the Axom project focuses on three central tenets:

  * Employ robust, commonly-used tools and don't re-invent something that already exists.
  * Apply tools in ways that non-experts find them easy to use.
  * Strive for automation and reproducibility.

The main interaction hub for Axom developers is the **Atlassian
tool suite** on the Livermore Computing Collaboration Zone (CZ). These tools
can be accessed through the `MyLC Portal <https://lc.llnl.gov>`_.
Developer-level access to Axom project spaces in these tools requires 
membership in the LC group 'axomdev'. If you are not in this group, and need 
to be, please send an email request to 'axom-dev@llnl.gov'.

The main Atlassian tools we use are listed below. Please navigate the links
provided for details about how we use them and helpful information about 
getting started with them.

* **Confluence.**  We use the `Axom Confluence space <https://lc.llnl.gov/confluence/display/ASCT>`_ for team discussion (e.g., hashing out design ideas), maintaining meeting notes, etc.

* **Bitbucket.** We use the `Axom Bitbucket project <https://lc.llnl.gov/bitbucket/projects/ATK>`_ to manage our Git repository which contains the Axom source code, build configurations, scripts, test suites, documentation, etc.

  * See :ref:`bitbucket-label` for more information about how we use Git and Bitbucket.

* **JIRA.** We use the `Axom JIRA project <https://lc.llnl.gov/jira/projects/ATK>`_ for issue tracking and release planning.

  * See :ref:`jira-label` for more information about how we use JIRA.

* **Bamboo.** We use two Bamboo projects for continuous integration, automated testing, documentation generation, etc.: `Axom CZ Bamboo project <https://lc.llnl.gov/bamboo/browse/ASC>`_ and `Axom RZ Bamboo project <https://rzlc.llnl.gov/bamboo/browse/ASC>`_

  * See :ref:`bamboo-label` for more information about how we use Bamboo.
