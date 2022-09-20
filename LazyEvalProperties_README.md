

Testing basic method of "lazy evaluation" or "evaluation on read" of properties.  This moves calculations to the GUI thread however, potentially slowing it down, especially if calcualation is longer than a few ms.  

The goal is to avoid unneccesary computation of data which won't be displayed, or only calculate data once which is used in multiple places in the GUI. 

Data is provided by DataItem objects (QObjects with properties that could be members of a data model), each of which calculates its data value based on one DataSource's input data value, which is expected to change periodically or spuriously at any time. 

A Loader object is used to defer any reads by the GUI if an item has been omitted by e.g. the ListView.

Another option is to use the "visible" QML property of the data item view delegates to trigger calculation.  

Change the macros at the top of DataItem.h to select "calculate on read" or "on visible" or disable both to do calculations of all DataItems whenever source data updates (on update signal from DataSource). 

There is also an option to precalculate DataItem values in the DataItem constructor for the first N items created.  This can be used to move the calculation to creation time rather than in the GUI display if you
expect a certain number of DataItem views to likely be used initially (e.g. in the ListView).  
