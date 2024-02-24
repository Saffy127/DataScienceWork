import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Load a dataset (for this example, we'll use the built-in Seaborn 'tips' dataset)
df = sns.load_dataset('tips')

# Data Cleaning (for this dataset, let's assume it's already clean)

# Exploratory Data Analysis (EDA)
# Let's visualize the relationship between total bill and tip
sns.scatterplot(x='total_bill', y='tip', data=df)
plt.title('Total Bill vs. Tip')
plt.xlabel('Total Bill')
plt.ylabel('Tip')
plt.show()

# Let's also look at a correlation matrix to see how variables are related
corr = df.corr()
sns.heatmap(corr, annot=True, cmap='coolwarm')
plt.show()

# Building a Simple Linear Regression Model to predict tips based on total bill
# Prepare data
X = df[['total_bill']]  # Predictor variable
y = df['tip']            # Response variable

# Split data into training and test sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Initialize and train the model
model = LinearRegression()
model.fit(X_train, y_train)

# Predictions
y_pred = model.predict(X_test)

# Evaluate the model
mse = mean_squared_error(y_test, y_pred)
print(f'Mean Squared Error: {mse}')

# Visualize the regression
plt.scatter(X_test, y_test, color='black', label='Actual data')
plt.plot(X_test, y_pred, color='blue', linewidth=3, label='Linear regression')
plt.title('Test Data vs Model Prediction')
plt.xlabel('Total Bill')
plt.ylabel('Tip')
plt.legend()
plt.show()
