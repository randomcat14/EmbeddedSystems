# EmbeddedSystems
Github for Embedded Systems Group

An image of the system architecture has been uploaded and the link to the System Architecture Diagram can be found here : https://drive.google.com/file/d/1wCjQwwzs13EIjpQw_Az2l4dP2CjA0QqB/view?usp=sharing


## Basic Git commands

In your terminal:

The first 3 steps should be done very often to save your progress. There should be a lot of commits because that way you save the progress as you go and can go back to a previous version if something goes wrong.
Steps 4 onwards are only done when you are done with whatever fix or creation you needed and are done with that section. To push all the code and merge you do all steps starting from 1 to 5. 

git checkout yourbranch  (to checkout and work on whatever branch you want)

### To create a branch you have 2 options:
Option 1.) git branch yourNewBranch ENTER (Creates the new branch but you are still in your current branch)
           git push -u origin yourNewBranch ENTER (Pushes the new branch to GitHub)

Option 2.) git checkout -b yourNewBranch ENTER (creates the new branch, pushes it into GitHub and puts you in the new branch)
           Then click the publish button on the left hand side

### To delete a branch:
git branch -d branchYouWantToDelete ENTER

git fetch -p ENTER (cleans your computer of branches that were deleted by other people)

### This section is for you to constantly save your progress.
1.) git branch ENTER (This will return the branch that you are in. Check that you are in the correct branch)

2.) git add . ENTER (This stages all of your changes and prepares them for the save)

3.) git commit -m “Here you will write your commit message and explain the changes that you made, it can be as long you want but there must be at least one character in this message or it won’t let you commit” ENTER

### This section is to upload your changes to the branch in Github with your section code. 

4.) git pull ENTER (This will grab the most up to date code from your branch that github has, it will not include the changes that you made that have not been aded yet. It should say that you are up to date. If it does not say that, that means that someone made an update while you were making your update and you need to look at all the conflicts and fix them before Github will let you upload your code. Solve the conflicts and do steps 1-4 again.)

5.)git push ENTER (Pushes your code into the master version of whatever branch you were working on)

### This section, is for when you are ready to merge your section with the other sections i.e. sensors code with database code.
6.)git pull origin Dev ( if it says you are up to date you are good. If it says that there are merge conflicts, go to the pages where those conflicts are and VS code will tell you what lines are conflicting. All of the conflicts must be resolved)

6.1) if there were conflicts with Dev and you just finished solving them, do steps 1-6 again 

7.) Once there are no more merge conflicts in your pull from Dev, create a pull request on GitHub. This must be done every time you are pushing code between branches


### This section is to check if the merge request was successfull.

8.) Make sure the pull request has been approved and merged on Github

9.) git checkout Dev ENTER

10.)git pull ENTER (to update the Dev branch in your computer with the Dev branch in Github)

11.)Now run the code to make sure that everything is well and up to date

### To push code from Dev to master(main), you do all of the same steps but put master as the branch in steps 8-9
