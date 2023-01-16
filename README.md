# Things Completed:

1. Minesweeper Game Logic
2. Game UI generation with slate
3. Game events flowing though based on clicks/typing inputs

# DEMO:

You can open the plugin from the toolbar and play around with UI. A lot of verification is done in the logs so it'd be 
nice to have that open. After updating the grid size you need to close and reopen the plugin window to see the new grid. 
The reasoning behind that is explained below.

# TODO:

Slate doesn't have a tick like gameobjects do. The plan I started was with refreshing the UI manually based on some event
triggers. But there's some issue with updating the SDockTab directly with new content which I am not able to figure out. 
If I had more time I might be able to fix that. 


Thanks.