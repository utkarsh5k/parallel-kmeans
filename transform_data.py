import pandas as pd

data = pd.read_csv("data/pollution_us_2000_2016.csv", header = 0, nrows = 10000)

data.rename(columns = {'Unnamed: 0' : 'ID'}, inplace = True)
print "Columns in the data"
print data.columns.values

"""
array(['ID', 'State Code', 'County Code', 'Site Num', 'Address', 'State',
       'County', 'City', 'Date Local', 'NO2 Units', 'NO2 Mean',
       'NO2 1st Max Value', 'NO2 1st Max Hour', 'NO2 AQI', 'O3 Units',
       'O3 Mean', 'O3 1st Max Value', 'O3 1st Max Hour', 'O3 AQI',
       'SO2 Units', 'SO2 Mean', 'SO2 1st Max Value', 'SO2 1st Max Hour',
       'SO2 AQI', 'CO Units', 'CO Mean', 'CO 1st Max Value',
       'CO 1st Max Hour', 'CO AQI'], dtype=object)
"""

data.drop(['ID', 'State Code', 'County Code', 'Site Num', 'Address', 'State', 'County',
 'City', 'Date Local', 'NO2 Units', 'O3 Units', 'SO2 Units', 'CO Units'], axis = 1, inplace = True)

print "Values containing NaN"
for column in data:
    if data[column].isnull().any():
        print column

"""
SO2 AQI
CO AQI
"""

data['SO2 AQI'].fillna(data['SO2 AQI'].mean(), inplace = True)
data['CO AQI'].fillna(data['CO AQI'].mean(), inplace = True)

data.to_csv('data/pollution_new_small.csv')
print "Written to CSV"
