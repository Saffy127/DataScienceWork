import pandas as pd
import matplotlib.pyplot as plt

# Load data
data = pd.read_csv('funny_sales_data.csv')

# Inspect the first rew rows
print(data.head())

# Summary statistics
print(data.describe())

# Calculate total sales
data['Total Sales'] = data['Price'] * data['Units Sold']

# Aggregate sales by date
sales_over_time = data.groupby('Date')['Total Sales'].sum()

# Visualization
plt.figure(figsize=(10, 6))
sales_over_time.plot(kind='line')
plt.title('Sales Over Time')
plt.xlabel('Date')
plt.ylabel('Total Sales')
plt.show()
