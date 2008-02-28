*******************************************************************************
$Id$

quicktabs

Description:
-------------------------------------------------------------------------------

  This module provides a form for admins to create a block of tabbed content by
selecting first the desired number of tabs and then selecting either an existing
view or an existing block as the content of each tab. Arguments can be passed if
a view is selected.



Installation & Use:
-------------------------------------------------------------------------------

1.  Enable module in module list located at administer > build > modules.
2.  Go to admin/settings/quicktabs to select a style for your tabs
3.  Go to admin/build/quicktabs and click on the "New QT block" local task tab.
4.  Add a title for your block and select the desired number of tabs
5.  For each tab, enter a title. Select either 'block' or 'view' from the Type drop
    down and then select the required block/view.
6.  Once you have defined all the tabs, click 'Next'.
7.  You will be taken to the admin/build/block screen where you should see yor new tabbed block listed.
8.  Configure & enable it as required.




Author:
-------------------------------------------------------------------------------

Katherine Bailey <katherine@raincitystudios.com>
http://drupal.org/user/172987
Tab Styles provided by Hubert Florin and Steve Krueger


TODO
-------------------------------------------------------------------------------

+ fix preview display of tab styles
+ add the ability to add/remove tabs from a quick tabs block when editing