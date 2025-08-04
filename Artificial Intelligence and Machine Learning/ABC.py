from abc import ABC, abstractmethod
from sklearn.metrics import accuracy_score, r2_score

class BaseEstimator(ABC):
    """The base class for all estimators in the library."""

    def get_params(self) -> dict:
        """Gets parameters for this estimator."""
        return {key: getattr(self, key) for key in self._get_param_names()}

    def set_params(self, **params):
        """Sets the parameters of this estimator."""
        for param, value in params.items():
            setattr(self, param, value)
        return self

    @classmethod
    def _get_param_names(cls):
        import inspect
        init_signature = inspect.signature(cls.__init__)
        return [p.name for p in init_signature.parameters.values() if p.name != 'self' and p.kind == p.POSITIONAL_OR_KEYWORD]

    @abstractmethod
    def fit(self, X, y=None):
        """Fits the model to the data. Must return self."""
        pass

class Transformer(BaseEstimator):
    """Base class for all transformers."""

    @abstractmethod
    def transform(self, X):
        """Transforms the data X."""
        pass

    def fit_transform(self, X, y=None):
        """Fits to data, then transforms it."""
        self.fit(X, y)
        return self.transform(X)
    
class SupervisedModel(BaseEstimator):
    """Base class for all supervised learning models."""

    @abstractmethod
    def fit(self, X, y):
        """Fits the model using features X and labels y."""
        pass

    @abstractmethod
    def predict(self, X):
        """Makes predictions for new data X."""
        pass

class Classifier(SupervisedModel):
    """Base class for all classifiers."""

    @abstractmethod
    def predict_proba(self, X):
        """Returns probability estimates for the data X."""
        pass

    def score(self, X, y) -> float:
        """Returns the mean accuracy on the given test data and labels."""
        return accuracy_score(y, self.predict(X))
    
class Regressor(SupervisedModel):
    """Base class for all regressors."""

    def score(self, X, y) -> float:
        """Returns the coefficient of determination R^2 of the prediction."""
        return r2_score(y, self.predict(X))
    
class UnsupervisedModel(BaseEstimator):
    """Base class for all unsupervised learning models."""

    @abstractmethod
    def fit(self, X, y=None):
        """Fits the model using features X."""
        pass

class Clustering(UnsupervisedModel):
    """Base class for all clustering algorithms."""
    
    labels_: list

    def fit_predict(self, X, y=None):
        """Fits to data, then returns the cluster labels."""
        self.fit(X, y)
        return self.labels_