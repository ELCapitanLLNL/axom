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

.. _jira-label:

*******************************************
JIRA: Issue Tracking and Release Cycles
*******************************************

We use our `Axom JIRA project space <https://lc.llnl.gov/jira/browse/ATK>`_ 
for issue tracking and work planning. In JIRA, you can create issues, edit
them, comment on them, assign them to individuals, check issue status, group 
them together for sprint development, and search for issues in various ways.

This section describes Axom software development cycles and basic issue work 
flow. 

.. _releasecycle-label:

========================================
Sprint Cycles and Work Planning
========================================

The Axom project plans work for quarterly (3 month) sprint cycles. Three
months is long for typical sprint-based development. However, we find that
it works well for our project where multiple software components are under
development concurrently by giving component developers flexibility to 
plan and coordinate work with other components in a way that works best for
them.

.. note:: Our three-month sprint development cycles do not necessarily 
          coincide with software releases. We may do multiple releases in
          a sprint or a single release may contain work from more than one 
          sprint. Our releases are driven by the frequency with which it makes
          sense to get new developments to our users.

The Axom sprint cycles align with the quarters of the LLNL fiscal year:

* Q1: October 1 -- December 31
* Q2: January 1 -- March 31
* Q3: April 1 -- June 30
* Q4: July 1 -- September 30

A few weeks before the start of one of these cycles, the team begins to 
discuss and prioritize the contents of our JIRA issue backlog. Those issues 
we target for the upcoming sprint cycle are moved into a corresponding
*version*. Here, *version* refers to the JIRA feature that allows a team to
create a collection of issues and track progress on them. Typically, we create
a named version, such as "Due 6/30/2017", for each quarterly sprint.

At the end of one sprint cycle and before starting the next one, we 
decide which issues that were not completed in the cycle we should move to the 
next. Hopefully, there are not many and typically we will propagate all 
such unresolved issues to the next sprint cycle. Before we start the next 
cycle, we meet as a team to make sure all issues in the associated version 
are assigned and that everyone is comfortable with their assignments.

To start a new sprint cycle, we create a new *sprint board* in JIRA and move
all issues in the upcoming version to it. We configure the board so that issues
are sorted into horizontal "swimlanes" one for each developer. There are 
three vertical columns on the sprint board that intersect the swimlanes to
indicate issues that: have not been started, are in progress, are done.
Then, we start the sprint; i.e., the new sprint cycle.

Note that as development occurs during a sprint, work will be reviewed and
merged from feature branches to the develop branch using pull requests. 
Reviewing work in smaller increments is much easier than reviewing everything 
at once. 

Depending on the need to make changes available to users, we may 
merge the develop branch into the master branch and tag new release numbers
multiple times during a release cycle. We may also choose to merge develop into 
master and tag a new release number at the end of a sprint. For a description
of how the master and develop branches interact, see :ref:`gitflow-label`.


.. _issueworkflow-label:

================
Issue Workflow 
================

We have customized our JIRA issue workflow to make it simple and easy to 
understand. This section explains key elements of the workflow and how to
use it.

Issue states
-------------

For the Axom project, each issue has three possible states:

* Open.
    Every issue starts out in an open state. An open issue can
    be assigned to someone or left unassigned. When an issue is assigned, this
    means that the assignee owns the issue and is responsible for working
    on it. An open issue that is left unassigned means that it has not been 
    been discussed or reviewed, or we have not decided how to act on it. 
    In general, an open issue is not being worked on.
* In Progress.
    An issue in progress is one that is actively being worked on.
* Closed.
    When an issue is closed, work on it has been completed, or
    a decision has been made that it will not be addressed.

An 'open' issue can transition to either 'in progress' or 'closed'. An 'in 
progress' issue can transition to either 'open' (work on it has stopped, 
but it is not finished) or 'closed'. Finally, a 'closed' issue
can be re-opened, which changes its state to 'open'. The complete issue workflow
is shown in the figure below.

.. figure:: jira-issue.png

   This figure shows allowed state transitions in our JIRA issue workflow.


Creating a new issue
---------------------

To create a new issue, click the 'Create' button at the top of the Axom
JIRA project page and enter information in the issue fields. Filling in the
fields properly helps team members search through project issues more easily.
Note that issue fields marked with a red asterisk are required -- they must 
be set to create a new issue. Other fields are not required, but may be used 
to include helpful information. The main issue fields we use are:

  Project
    Axom will show up as the default. You shouldn't need
    to change this.
  Issue Type
    We use only three issue types: *Bug*, *New Feature*, and
    *Task*. A bug is something broken that needs to be fixed. A new feature
    is something that adds functionality, enhances an interface, etc. Task 
    is a "catch-all" issue type for any other issue.
  Summary
    Provide a short descriptive summary. A good (and brief)
    summary makes it easy to scan a list of issues to find one you are
    looking for.
  Priority
    Select an appropriate issue priority to identify its level
    of importance or urgency. Clicking on the question mark to the right of
    the priority field provides a description of each option.
  Components
    Each issue is labeled with the Axom component it applies to. 
    Component labels also include things like: build system, documentation, 
    testing, etc.
  Assignee
    Unless you are certain which team member should be assigned
    an issue, choose 'Unassigned'. This will indicate that the issue requires
    discussion and review before it is assigned. The default assignee is the
    owner of the component you chose earlier if you make no choice.
  Reporter
    Unless you explicitly enter someone in this field, you, as
    the issue creator, will be the reporter. This is the correct choice in
    almost all cases.
  Description
    The description field should be used to include important
    details about the issue that will help the developer who will work on it.
  Environment
    The environment field can be useful when an issue affects a particular
    compiler or platform.
  Epic-link
    An *epic* is a special issue type in the Agile methodology that is used to
    define a larger body of work that can be comprised of many issues. 
    **However, that's not what we use epics for. See note below.**

You may also use the other fields that appear if you think they will help
describe the issue. However, the team seldom uses fields apart from the list
above.

.. note:: We use epics in JIRA and link our issues to them to get a convenient 
          label on each each issue when we look at a sprint board or the issue 
          backlog. We have an epic for each of our components and the epic
          name matches the component name for this purpose. 
    
Starting and stopping work on an issue
---------------------------------------

When you begin work on an issue, you should note this by changing its state
from 'open' to 'in progress'. There are two ways to perform this transition.
The first is to open the issue and click the 'Start Progress' button at the 
top of the issue menu. Alternatively, if the issue is in the 'open' column 
on a sprint board, you can drag and drop it into the 'in progress' column.
Either way changes the issue status to 'in progress'.

If there is still work to do on the issue, but you will stop working on it
for a while, you can click the 'Stop Progress' button at the top of the
issue. Alternatively, if the issue is in the 'in progress' column on a sprint 
board, you can drag and drop it into the 'open' column.
Either way changes the issue status to *open*.

Closing an issue
-----------------

When work on an issue is complete (including testing, documentation, etc.), 
or the issue will not be addressed, it should be closed. To close an issue, 
click the 'Close' button and select the appropriate issue resolution. There 
are two options: *Done* and *Won't Fix*.  'Done' means that the issue is 
resolved. 'Won't Fix' means that the issue will not be addressed for some 
reason.

When closing an issue, adding information to the 'Comment' field is
helpful. For example, when an issue is closed as 'Won't Fix', it is helpful to
enter a brief explanation as to why this is so.

Issue assignee
--------------

Note that an assigned issue can be assigned to someone else to work on it.
An assigned issue can also be set back to 'Unassigned' if it needs further
discussion by the team.

JIRA tips
----------

Here are some links to short videos (a couple of minutes each) that
demonstrate how to use JIRA features:

   * `JIRA Instant Search Bar Demo <https://www.youtube.com/watch?v=ZmACxhzXLco&list=PLlALqRAjvdnGB_T0GAB1Fk2rVZgnJJAOa&index=3>`_
   * `JIRA System Files Demo <https://www.youtube.com/watch?v=O08oySq043w&list=PLlALqRAjvdnGB_T0GAB1Fk2rVZgnJJAOa&index=4>`_
   * `Creating and Editing JIRA Issues <https://www.youtube.com/watch?v=EsQ__dR6Nrw&list=PLlALqRAjvdnGB_T0GAB1Fk2rVZgnJJAOa&index=5>`_
